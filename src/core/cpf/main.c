#include <signal.h>
#include <libconfig.h++>

#include <rte_cycles.h>
#include <rte_ethdev.h>
#include <rte_ip.h>
#include <rte_udp.h>
#include <rte_malloc.h>

#include "Common/globals.h"
#include "Common/logger/Logger.h"
#include "src/cpf.h"

using namespace libconfig;
using namespace std;

#define CPU_SOCKETS 1
#define LOCK_FILE "core.lck"

#define REP_MEMPOOL_SIZE ((1 << 15) - 1)
#define NB_MBUF ((1 << 23) - 1)
const unsigned long MAX_RING_SIZE = (1 << 23);
const unsigned long CPF_REP_RING_SIZE = (1 << 23);

unsigned int curr_cores[CPU_SOCKETS];

#define MEMPOOL_CACHE_SIZE 256
static uint16_t nb_rxd = (1 << 12);
static uint16_t nb_txd = (1 << 12);


volatile bool force_quit = false;
struct rte_mempool *global_mempool = NULL;

const size_t PAYLOAD_OFFSET =
    sizeof(struct ether_hdr) + sizeof(struct ipv4_hdr) + sizeof(struct udp_hdr);

const size_t CPF_ID_OFFSET =
    sizeof(double) + sizeof(lgclock_t) + sizeof(size_t) + sizeof(guti_t); 

void *setup_ports(int nb_workers, struct rte_eth_conf port_conf);

struct cpf_args
{
  int id;
  struct rte_ring *rx_ring;
  struct rte_ring *rep_ring_recv;
  struct CPFConfig *cpf_config;
  vector< map<unsigned long int, double>> *pcts;
  vector<float> *cpu_loads;
  vector<int> replica_ids;
  int propagation_delay;
};

static void lock_cpf()
{
  FILE *fptr;
  fptr = fopen(LOCK_FILE, "w");
  fclose(fptr);
}

static void unlock_cpf() { remove(LOCK_FILE); }

void check_port_status(uint8_t portid) {
#define CHECK_INTERVAL 100 /* 100ms */
#define MAX_CHECK_TIME 50  /* 5s (50 * 100ms) in total */
  uint8_t count, port_up, print_flag = 0;
  struct rte_eth_link link;

  printf("\nChecking link status");
  fflush(stdout);
  port_up = 1;
  for (count = 0; count <= MAX_CHECK_TIME; count++) {
    if (force_quit) return;
    memset(&link, 0, sizeof(link));
    rte_eth_link_get_nowait(portid, &link);
    /* print link status if flag set */
    if (print_flag == 1) {
      if (link.link_status)
        printf(
            "Port %d Link Up - speed %u "
            "Mbps - %s\n",
            (uint8_t)portid, (unsigned)link.link_speed,
            (link.link_duplex == ETH_LINK_FULL_DUPLEX) ? ("full-duplex")
                                                       : ("half-duplex\n"));
      else
        printf("Port %d Link Down\n", (uint8_t)portid);
    }
    /* clear port_up flag if link down */
    if (link.link_status == ETH_LINK_DOWN) {
      port_up = 0;
    }

    /* after finally printing all link status, get out */
    if (print_flag == 1) break;

    if (port_up == 0) {
      printf(".");
      fflush(stdout);
      rte_delay_ms(CHECK_INTERVAL);
    }

    /* set the print_flag if all ports up or timeout */
    if (port_up == 1 || count == (MAX_CHECK_TIME - 1)) {
      print_flag = 1;
      printf("done\n");
    }
  }
}

static unsigned int get_next_lcore(int sock_id)
{
  curr_cores[sock_id] += CPU_SOCKETS;
  return curr_cores[sock_id];
}

/* 
  This function is responsible for configuring and creating CPFs -- the main processing unit of Neutrino. 
*/

static void launch_CPF(struct cpf_args *args)
{

  char name[64];
  CPF *new_cpf = new CPF((uint16_t)(args->id), args->rx_ring, args->rep_ring_recv,
                         args->id + 9091, args->cpf_config,
                         args->pcts, args->cpu_loads, 
                         args->replica_ids, args->propagation_delay);

  sprintf(name, "rep_mempool_%u", args->id);
  struct rte_mempool *rep_mempool =
      rte_mempool_create(name, REP_MEMPOOL_SIZE, sizeof(State),
                        RTE_MEMPOOL_CACHE_MAX_SIZE, 0, NULL, NULL, NULL, NULL,
                        SOCKET_ID_ANY, MEMPOOL_F_SP_PUT | MEMPOOL_F_SC_GET);

  new_cpf->setRepMempool(rep_mempool);
  new_cpf->createDataWorker();

  if (args->cpf_config->replication != 0) {

    sprintf(name, "Rep_%d", args->id);
    struct rte_ring *cpf_rep_ring = rte_ring_create(
        name, CPF_REP_RING_SIZE / 16, rte_socket_id(), RING_F_SP_ENQ);

    if (cpf_rep_ring == NULL) {
      rte_exit(EXIT_FAILURE, "Cannot create cpf replication ring!");
    }

    new_cpf->setRepRing(cpf_rep_ring);

    // Replication message sender thread.
    rte_eal_remote_launch(runCPFSender, (void *)new_cpf,
                          get_next_lcore(rte_socket_id()));
    
    // Replication message processing thread,
    rte_eal_remote_launch(runCPFReceiver, (void *)new_cpf,
                          get_next_lcore(rte_socket_id()));
  }

  // Procedure message reception thread
  rte_eal_remote_launch(runDataWorker, (void *)new_cpf,
                        get_next_lcore(rte_socket_id()));
}

void enqueue_cpf_ring(struct rte_mbuf *pkt, struct rte_ring **rings){
  
  uint8_t *payload = rte_pktmbuf_mtod_offset(pkt, uint8_t *, PAYLOAD_OFFSET);

  // Finding for each message the selected CPF. This bit was set on CTA.
  uint8_t cpf_id = *(payload + CPF_ID_OFFSET);

  // Sending the message to the approriate CPF.
  rte_ring_enqueue(rings[cpf_id], pkt);
}

/* 
  This thread is responsible for the following tasks,
  1. Consuming RX from the CTA server.
  2. Finding which message belongs to which CPF.
  3. Sending messages to the associated CPFs through ring buffers.
 */

static void do_rx(void *arg) {
    struct rte_ring **rx_rings = (struct rte_ring **) arg;
    struct rte_mbuf *pkts_burst[MAX_PKT_BURST];
    int nb_rx;
    int count = 0;

    while (!force_quit){

      // Consuming RX from CTA
      nb_rx = rte_eth_rx_burst((uint8_t)PORT, 0, pkts_burst, MAX_PKT_BURST);

      if (nb_rx == 0) {
        continue;
      }

      for (int i = 0; i < nb_rx; i++){
        enqueue_cpf_ring(pkts_burst[i], rx_rings);
      }
      count += nb_rx;

    }

    printf("CPF main RX thread recv count: %d\n", count);
}

static void do_rx_rep(void *arg) {
  
  struct rte_ring **rep_rings = (struct rte_ring **) arg;
  struct rte_mbuf *pkts_burst[SYNC_BATCH_SIZE];
  int nb_rx_rep, count = 0;

  while (!force_quit){
    nb_rx_rep = rte_eth_rx_burst((uint8_t)REPLICATION_PORT, 0, pkts_burst, SYNC_BATCH_SIZE);

    if (nb_rx_rep == 0){
      continue;
    }

    for (int i = 0; i < nb_rx_rep; i++){
      enqueue_cpf_ring(pkts_burst[i], rep_rings);
    }
    count += nb_rx_rep;

  }
}


static void setup_ports(unsigned int nb_workers) {
  uint16_t nb_ports = rte_eth_dev_count();
  if (nb_ports < 2)
    rte_exit(EXIT_FAILURE, "We need atleast 2 ports to work.\n");

  static struct rte_eth_conf port_conf;
  port_conf.rxmode.split_hdr_size = 0;
  port_conf.txmode.mq_mode = ETH_MQ_TX_NONE;

  setup_ports(nb_workers, port_conf);
  check_port_status(PORT);

  return;
}

static void
signal_handler(int signum)
{
	if (signum == SIGINT || signum == SIGTERM)
	{
		printf("\n\nSignal %d received, preparing to exit...\n",
			   signum);
		force_quit = true;
	}
}

void *setup_ports(int nb_workers, struct rte_eth_conf port_conf) {

  uint16_t portids[TOTAL_PORTS] = {PORT, REPLICATION_PORT};
  unsigned int nb_mbufs;
  int ret;
  struct rte_eth_rxconf rxq_conf;
  struct rte_eth_txconf txq_conf;

  struct rte_eth_conf local_port_conf = port_conf;
  struct rte_eth_dev_info dev_info;
  
  nb_mbufs = (1 << 23) - 1;
  struct rte_mempool *rx_mempool =
      rte_pktmbuf_pool_create("mbuf_pool", nb_mbufs, MEMPOOL_CACHE_SIZE, 0,
                              RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());
  if (rx_mempool == NULL) rte_exit(EXIT_FAILURE, "Cannot init mbuf pool\n");

  for (int i = 0; i < TOTAL_PORTS; i++){
    uint16_t portid = portids[i];

    printf("Initializing RX port %u...\n", portid);
    fflush(stdout);

    // Support for multi segment packets.
    rte_eth_dev_info_get(portid, &dev_info);
    if (dev_info.tx_offload_capa & DEV_TX_OFFLOAD_MBUF_FAST_FREE)
      local_port_conf.txmode.offloads |= DEV_TX_OFFLOAD_MBUF_FAST_FREE;

    // Setup port for rx and tx queues
    ret = rte_eth_dev_configure(portid, 1, nb_workers, &local_port_conf);
    if (ret < 0)
      rte_exit(EXIT_FAILURE, "Cannot configure device: err=%d, port=%u\n", ret,
              portid);

    /* init RX queues */
    rxq_conf = dev_info.default_rxconf;
    rxq_conf.offloads = local_port_conf.rxmode.offloads;

    ret = rte_eth_rx_queue_setup(portid, 0, nb_rxd, rte_eth_dev_socket_id(portid),
                                &rxq_conf, rx_mempool);
    if (ret < 0)
      rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup:err=%d, port=%u\n", ret,
              portid);


    txq_conf = dev_info.default_txconf;
    txq_conf.offloads = local_port_conf.txmode.offloads;

    for (int i = 0; i < nb_workers; i++) {
        ret = rte_eth_tx_queue_setup(portid, i, nb_txd,
                                    rte_eth_dev_socket_id(portid), &txq_conf);
        if (ret < 0)
          rte_exit(EXIT_FAILURE, "rte_eth_tx_queue_setup:err=%d, port=%u\n", ret,
                  portid);
    }

    /* Start device */
    ret = rte_eth_dev_start(portid);
    if (ret < 0)
      rte_exit(EXIT_FAILURE, "rte_eth_dev_start:err=%d, port=%u\n", ret, portid);

    rte_eth_promiscuous_enable(portid);

    printf("RX port %d is up!\n", portid);
    printf("TX port %d is up!\n", portid);
    fflush(stdout);

  }  
}

vector<int> extract_replicas(Setting &replicas){
  vector<int> replica_ids;

  for (int j = 0; j < replicas.getLength(); j++){
    replica_ids.push_back(replicas[j]);
    replica_ids[j] = replica_ids[j] - 1;
  }

  return replica_ids;
}

int main(int argc, char **argv){

  int ret;
  ret = rte_eal_init(argc, argv);
  if (ret < 0) rte_exit(EXIT_FAILURE, "Invalid EAL arguments\n");

  Config cfg;
  cfg.readFile("cpf_config.cfg");

  // Read the configuration file
  int serializer = cfg.lookup("serializer");
  int replication = cfg.lookup("replication");
  int number_of_cpf = cfg.lookup("number_of_cpf");
  int propagation_delay = cfg.lookup("propagation_delay");
  int entity_id = cfg.lookup("entity_id");
  Setting &local_cpfs = cfg.lookup("local_cpfs");
  Setting &remote_cpfs = cfg.lookup("remote_cpfs");

  int length;

  if (entity_id == 1){
    length = local_cpfs.getLength();
  } else if (entity_id == 2){
    length = remote_cpfs.getLength();
  }

  setup_ports(number_of_cpf);
  global_mempool = rte_pktmbuf_pool_create("Global-mempool", NB_MBUF,
											 MEMPOOL_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE,
											 rte_socket_id());
	if (global_mempool == NULL)
		rte_exit(EXIT_FAILURE, "Cannot init mbuf pool\n");

  signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

  // Create the files to record the procedure completion times.
  FILE *attach_file = InitLogger("logs/CPF_attach_logs.txt");
  FILE *handover_file = InitLogger("logs/CPF_handover_logs.txt");
  FILE *service_file = InitLogger("logs/CPF_service_logs.txt");

  struct cpf_args cpf_args;
  vector <map<unsigned long int, double>> pcts;

  map<unsigned long int, double> attach_pcts;
  map<unsigned long int, double> handover_pcts;
  map<unsigned long int, double> service_pcts;

  pcts.push_back(attach_pcts);
  pcts.push_back(handover_pcts);
  pcts.push_back(service_pcts);

  vector<float> cpu_loads;
  vector<float> lb_cpu_loads;

  struct CPFConfig *conf = malloc(sizeof(CPFConfig));
  conf->serializer = serializer;
  conf->replication = replication;
  conf->number_of_cpfs = number_of_cpf;
  conf->cpu_loads = &lb_cpu_loads;

  cpf_args.rx_ring = NULL;
  cpf_args.cpf_config = conf;
  cpf_args.pcts = &pcts;
  cpf_args.cpu_loads = &cpu_loads;
  cpf_args.propagation_delay = propagation_delay;


  // RX rings for each cpf.
  unsigned ring_size = MAX_RING_SIZE / 4;
  struct rte_ring *rx_rings[number_of_cpf];
  struct rte_ring *rep_rings_recv[number_of_cpf];

  int id;

  // Launch CPFs (either local or remote) after assigning them message and replication rings.  
  for (int i = 0; i < length; i++){
    char ring_name[32];
    sprintf(ring_name, "Dequeue_%d", i);

    if (entity_id == 1) {
      local_cpfs[i].lookupValue("cpf", id);
    } else if (entity_id == 2) {
      remote_cpfs[i].lookupValue("cpf", id);
    }
    cpf_args.id = id - 1;

    rx_rings[cpf_args.id] = rte_ring_create(ring_name, MAX_RING_SIZE, rte_socket_id(),
                               RING_F_SP_ENQ | RING_F_SC_DEQ);

    sprintf(ring_name, "Rep_Recv_%d", i);
    rep_rings_recv[cpf_args.id] = rte_ring_create(ring_name, MAX_RING_SIZE, rte_socket_id(),
                               RING_F_SP_ENQ | RING_F_SC_DEQ);

    cpf_args.rx_ring = rx_rings[cpf_args.id];
    cpf_args.rep_ring_recv = rep_rings_recv[cpf_args.id];

    if (entity_id == 1){
      cpf_args.replica_ids = extract_replicas(local_cpfs[i].lookup("replicas"));
    } else if (entity_id == 2) {
      cpf_args.replica_ids = extract_replicas(remote_cpfs[i].lookup("replicas"));
    }

    launch_CPF(&cpf_args);
  }



  // Launch thread that receives messages from the CTA.
  rte_eal_remote_launch(do_rx, (void *)rx_rings,
                        get_next_lcore(rte_socket_id()));  
  
  // Launch thread that receives the replication messages.
  rte_eal_remote_launch(do_rx_rep, (void *)rep_rings_recv,
                        get_next_lcore(rte_socket_id()));  


  lock_cpf();

  uint16_t i;
  RTE_LCORE_FOREACH_SLAVE(i) {
    if (rte_eal_wait_lcore(i) < 0) {
      ret = -1;
    }
  }

  unlock_cpf();

  for (auto const &x : pcts[0])
  {
    WriteNumber((uint32_t)x.second, attach_file);
  }

  for (auto const &x : pcts[1])
  {
    WriteNumber((uint32_t)x.second, handover_file);
  }
  for (auto const &x : pcts[2])
  {
    WriteNumber((uint32_t)x.second, service_file);
  }


  return 0;

}