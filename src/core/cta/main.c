#include <signal.h>

#include <rte_cycles.h>
#include <rte_malloc.h>
#include <rte_hash_crc.h>

#include <mutex>
#include <unistd.h>
#include <libconfig.h++>

#include "Common/logger/Logger.h"
#include "NicInterface/NicInterface.h"
#include "mem_store/store_wrapper.h"
#include "init/initializer.hpp"
#include "cpf/cpf.h"

#define LOCK_FILE "core.lck"
#define CPU_SOCKETS 1
unsigned int curr_cores[CPU_SOCKETS];

typedef unsigned long long int nb_bytes_t;

// Number of threads which will dequeue the packets from
// general ring buffer and forward it to CPF.
using namespace libconfig;

volatile bool force_quit = false;
int pkt_logging = false;

#define RTE_HASH_ENTRIES_MAX 1 << 25
const unsigned long STORE_RING_SIZE = 1 << 20;

// Not the actuall size of the ring, it is divided into equal
// parts to create rings for dequeue threads.
// Number of dequeue threads is controlled by DEQUEUE_THREADS.
const unsigned long MAX_RING_SIZE = (1 << 25);
const unsigned long CPF_TX_RING_SIZE = 1 << 22;

struct rte_ring *rings[RX_DEQUEUE_THREADS];
struct rte_mempool *global_mempool = NULL;

static struct ether_addr ether_dst = {{0x68, 0x05, 0xca, 0x00, 0x00, 0x01}};
static struct ether_addr zero_mac = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

static inline uint16_t generate_packet(struct rte_mbuf *, uint8_t *, uint32_t);

NicInterface nicInterface;

struct cpf_args
{
  int id;
  int type;
  int sync_time;
  struct rte_ring **tx_rings;
  struct CTAConfig *cpf_config;
  vector<float> *cpu_loads;
};

/* 
  This thread is responsible for following tasks.
  1. Receive the responses coming from CPFs.
  2. Forward the messages to the transmission workers to return the
     messages back to the traffic generator.
 */

void *listen_on_tx(void *args) {
  
  struct LBRtnInfo *lb_rtn_info = (struct LBRtnInfo*) args;
  struct rte_ring **cpf_tx_rings = lb_rtn_info->lb_rtn_rings;

  uint64_t start_time, end_time;
  nb_bytes_t tx_bytes = 0;
  const uint64_t hz = rte_get_timer_hz();
  uint64_t nb_rx_local, nb_rx_remote;
  int ret;

  printf("Starting tx thread on lcore %d\n", rte_lcore_id());
  struct rte_mbuf *pkts_burst[MAX_PKT_BURST];

  int ring_index = 0;

  while (!force_quit)
  {
    // Receiving responses from the CPFs.
    nb_rx_local = rte_eth_rx_burst((uint8_t)LOCAL_CPF_PORT, 0, pkts_burst, MAX_PKT_BURST);
    nb_rx_remote = rte_eth_rx_burst((uint8_t)REMOTE_CPF_PORT, 0, pkts_burst+nb_rx_local, MAX_PKT_BURST);

    if (nb_rx_local == 0 && nb_rx_remote == 0) {
      continue;
    }


    // Forward the messages to transmission threads to return the messages to 
    // traffic generator.
    ret = rte_ring_enqueue_burst(cpf_tx_rings[ring_index], pkts_burst, 
                                 nb_rx_local+nb_rx_remote, NULL);
    if (unlikely(ret < nb_rx_local+nb_rx_remote)){
      printf("CTA response ring buffer full");
    }
    update_rx(LOCAL_CPF_PORT, nb_rx_local);
    update_rx(REMOTE_CPF_PORT, nb_rx_remote);
    ring_index = (ring_index + 1) % TX_DEQUEUE_THREADS;
    end_time = rte_get_timer_cycles();
  }

  double total_time = (double)(end_time - start_time) / hz;
  double tx_rate = (tx_bytes * 8 * 1e-9) / total_time;

  printf("Cycles: %0.4f\n", total_time);
  printf("Tx Rate %0.4f\n", tx_rate);
}

/* These workers are resposible for following tasks.
   1. Receive the messages from the 'listen_on_tx' 
      thread.
   2. Send these messages back to the traffic generator.
 */
void* flush_tx(void *args) {
  LBInfo *conf = (LBInfo *)args;
  uint64_t count;

  struct rte_mbuf *pkt_burst[MAX_PKT_BURST];
  unsigned int nb_rx;

  while(!force_quit) {


    // Receive messages from the ring buffers filled by the 'listen_on_tx' 
    // thread.
    nb_rx =
          rte_ring_dequeue_burst(conf->ring, (void **)pkt_burst, MAX_PKT_BURST, NULL);

    if (nb_rx == 0){
      continue;
    }


    // Send the messages back to the traffic generator.
    uint16_t to_send, sent;
    to_send = nb_rx;
    count += nb_rx;
    while (to_send != 0) {
      sent = rte_eth_tx_burst((uint8_t)TX_PORT, conf->id, pkt_burst + (nb_rx - to_send), to_send);
      to_send -= sent;
    }

    update_tx(TX_PORT, nb_rx);

  }

  printf("Count at CTA TX thread: %u\n", count);
}

static inline uint16_t generate_packet(struct rte_mbuf *buf, uint8_t *response,
                                       uint32_t size) {
  buf->ol_flags = PKT_TX_IPV4 | PKT_TX_IP_CKSUM | PKT_TX_UDP_CKSUM;
  buf->l2_len = sizeof(struct ether_hdr);
  buf->l3_len = sizeof(struct ipv4_hdr);
  buf->l4_len = sizeof(struct udp_hdr);

  struct ether_hdr *eth_hdr;
  struct ipv4_hdr *ip_hdr;
  struct udp_hdr *udp_hdr;
  struct tcp_hdr *tcp_hdr;
  uint16_t pkt_size = MAX_PKT_SIZE;

  buf->pkt_len = pkt_size;
  buf->data_len = pkt_size;

  eth_hdr = rte_pktmbuf_mtod(buf, struct ether_hdr *);
  if (is_zero_ether_addr(&zero_mac))
  {
    ether_addr_copy(&ether_dst, &eth_hdr->d_addr);
  }
  else
  {
    ether_addr_copy(&zero_mac, &eth_hdr->d_addr);
  }

  if (is_zero_ether_addr(&zero_mac))
  {
    ether_addr_copy(&zero_mac, &eth_hdr->s_addr);
  }
  else
  {
    ether_addr_copy(&zero_mac, &eth_hdr->s_addr);
  }

  eth_hdr->ether_type = rte_cpu_to_be_16(ETHER_TYPE_IPv4);

  ip_hdr = (struct ipv4_hdr *)(eth_hdr + 1);
  ip_hdr->type_of_service = 0;
  ip_hdr->fragment_offset = 0;
  ip_hdr->time_to_live = 64;
  ip_hdr->next_proto_id = IPPROTO_UDP;
  ip_hdr->packet_id = 0;
  ip_hdr->version_ihl = (1 << 6) + 5;
  ip_hdr->total_length = rte_cpu_to_be_16(pkt_size - sizeof(struct ether_hdr));
  ip_hdr->src_addr = 0xAABB;
  ip_hdr->dst_addr = 0xCCDD;
  ip_hdr->hdr_checksum = 0;

  udp_hdr = (struct udp_hdr *)(ip_hdr + 1);
  udp_hdr->src_port = 0xAABB;
  udp_hdr->dst_port = 0xCCDD;
  udp_hdr->dgram_cksum = 0;
  udp_hdr->dgram_len =
      rte_cpu_to_be_16(pkt_size - sizeof(struct ether_hdr) - sizeof(*ip_hdr));

  uint8_t *ptr = rte_pktmbuf_mtod_offset(buf, uint8_t *, PAYLOAD_OFFSET);

  rte_memcpy((void *)ptr, response, size);
  return pkt_size;
}

std::mutex mtx;

static inline size_t extract_payload_length(struct rte_mbuf *pkt){
  uint8_t *ptr;
  payloadLenExtractor ext;

  ptr = rte_pktmbuf_mtod_offset(pkt, uint8_t *, PAYLOAD_LENGTH_OFFSET);
  for (int j = 0; j < sizeof(size_t); j++){
    ext.buff[j] = ptr[j];
  }

  return ext.payload_length;

}

static inline void extract_payload_lengths(struct rte_mbuf **pkts, uint16_t nb_rx,
                                           size_t *payload_lengths){
  for (int i = 0; i < nb_rx; i++)
  {
    payload_lengths[i] = extract_payload_length(pkts[i]);  
  }
}

static inline void extract_gutis(struct rte_mbuf **pkts, uint16_t nb_rx,
                                 guti_t *gutis){
  
  uint8_t *ptr;
  gutiExtractor ext;

  for (int i = 0; i < nb_rx; i++)
  {
    ptr = rte_pktmbuf_mtod_offset(pkts[i], uint8_t *, GUTI_OFFSET);
    for (int j = 0; j < sizeof(guti_t); j++){
      ext.buff[j] = ptr[j];
    }
    gutis[i] = ext.guti;
    
  }

}

// Given a packet, this fucntion adds an always increasing logical clock to the packet header.
static inline void add_logical_clock(struct rte_mbuf **pkts, uint16_t nb_rx,
                                     nb_bytes_t *rx_bytes) {
  static lgclock_t lg_clock = 0;
  const static size_t lg_clock_offset = sizeof(lgclock_t);
  uint8_t *ptr;

  for (int i = 0; i < nb_rx; i++)
  {
    // Jump to logical clock ....
    ptr = rte_pktmbuf_mtod_offset(pkts[i], uint8_t *, PAYLOAD_OFFSET);

    logicalClockExtractor ext;
    ext.logical_clock = lg_clock;

    ptr = ptr + lg_clock_offset;

    for (int i = 0; i < sizeof(lgclock_t); i++)
    {
      *(ptr + i) = ext.buff[i];
    }

    lg_clock++;
    *rx_bytes += pkts[i]->pkt_len;
  }
}

int initial_msg_index(size_t payloads[MAX_PKT_BURST], int nb_rx){

  for (int i = 0; i < nb_rx; i++){
    if (payloads[i] == 423) {
      return i;
    }
  }

  return -1;
}

/* 
  processPkt workers are responsible for the following tasks,
  1. Receive burst messages from the 'recvPkts' thread.
  2. Log the received messages (if enabled).
  2. Search the consistent hash ring to find the appropriate CPF for
  each message.
  3. Sending each message to the CPFs through DPDK apis.
 */

void *processPkt(void *arg)
{
  printf("Starting a separate thread for sending packets to CPF on core %d\n",
         rte_lcore_id());

  struct LBInfo *lb_args = (struct LBInfo *)arg;
  int queue_id = lb_args->id;
  struct rte_ring *ring = lb_args->ring;

  CustomHeaderParser header_parser;
  CustomAppHeader header;
  struct rte_mbuf *pkt_burst[MAX_PKT_BURST];
  uint8_t *port;
  unsigned int nb_rx, i;

  uint64_t rx_count = 0;
  uint64_t rx_start_time = 0;

  const uint64_t hz = rte_get_timer_hz();
  double processing_cycles = 0, total_cycles = -1;
  double current_cycles = rte_get_timer_cycles();

  while (!force_quit)
  {

    // Dequeueing messages coming from the 'recvPkts' thread.
    nb_rx =
        rte_ring_dequeue_burst(ring, (void **)pkt_burst, MAX_PKT_BURST, NULL);

    if (total_cycles != -1){
      total_cycles += (double)(rte_get_timer_cycles() - current_cycles) / hz;
      current_cycles = rte_get_timer_cycles();
    } 

    if (nb_rx == 0)
      continue;

    if (unlikely(rx_count == 0))
    {
      rx_start_time = rte_get_timer_cycles();
      total_cycles = (double) (rte_get_timer_cycles() - current_cycles) / hz;
      current_cycles = rte_get_timer_cycles();
    }

    for (i = 0; i < nb_rx; i++)
    {
      port = rte_pktmbuf_mtod_offset(pkt_burst[i], uint8_t *, FLAG_OFFSET);

      uint8_t *to_copy = rte_pktmbuf_mtod_offset(pkt_burst[i], uint8_t *, PAYLOAD_OFFSET);
      header_parser.Parse(to_copy);

      header = header_parser.controlMessage.header;

      int payload_length = extract_payload_length(pkt_burst[i]);

      /* 
         Passing the messages to 'OnPayloadReceived to perform',
         1. Message logging.
         2. Consistent hash ring search.
      */
      nicInterface.OnPayloadReceived(pkt_burst[i], header, queue_id, *port);
    }

    rx_count += nb_rx;
    processing_cycles += (double) (rte_get_timer_cycles() - current_cycles) / hz;
  }

  mtx.lock();
  (lb_args->config->cpu_loads)->push_back((processing_cycles / total_cycles) * 100);
  mtx.unlock();
  return NULL;
}

/* This is the main function for RX thread.
   This fucntion reads the packets from the NIC using dpdk's internal function call,
   adds an always increasing logical clock and sends the complete bursts of packets
   to a dequeue-thread.
*/
static void recvPkts(void *arg)
{

  struct CTAConfig *config = (struct CTAConfig *)arg;

  int MAX_SIZE = 9999999999;
  int new_msgs_threshold = 500;

  if (!config->georep_experiment){
    new_msgs_threshold = MAX_SIZE;
  }

  double new_msgs_per_wait = 0;

  double total_cycles = 0;
  double cycles_wait = 40000000;

  guti_t start_guti = 0;
  guti_t end_guti = MAX_SIZE;

  int traffic_status = LOW_LOAD;

  uint64_t count = 0;
  uint64_t index = 0;
  unsigned drop_count = 0;
  uint64_t c_start = 0, c_end = 0;
  unsigned lcore_id;
  uint16_t nb_rx;
  unsigned int ret;
  struct rte_mbuf *pkt_burst[MAX_PKT_BURST];
  size_t payloads[MAX_PKT_BURST];
  guti_t gutis[MAX_PKT_BURST];
  nb_bytes_t rx_bytes = 0;
  uint8_t *ptr;

  const uint64_t hz = rte_get_timer_hz();
  lcore_id = rte_lcore_id();

  printf("Starting master thread for RX on lcore %u\n", lcore_id);
  while (!force_quit)
  {
    // Consumes RX
    nb_rx = rte_eth_rx_burst((uint8_t)RX_PORT, 0, pkt_burst, MAX_PKT_BURST);
    if (nb_rx == 0)
      continue;

    if (unlikely(count == 0))
      c_start = rte_get_timer_cycles();

    extract_payload_lengths(pkt_burst, nb_rx, payloads);
    extract_gutis(pkt_burst, nb_rx, gutis);

    add_logical_clock(pkt_burst, nb_rx, &rx_bytes);

    c_end = rte_get_timer_cycles();
    total_cycles = (double)(c_end - c_start);

    if (total_cycles > cycles_wait){
      if (new_msgs_per_wait > new_msgs_threshold){
          traffic_status = HIGH_LOAD;
      } else {
          if (traffic_status == HIGH_LOAD){
            traffic_status = DECREASED_LOAD;
          } else {
            traffic_status = LOW_LOAD;
          }
      }

      c_start = rte_get_timer_cycles();
      new_msgs_per_wait = 0;
    } else {
      for (int i = 0; i < nb_rx; i++){
        if ((payloads[i] == 423)){
          new_msgs_per_wait++;
        }
      }
    }

    if (traffic_status == HIGH_LOAD && start_guti == 0){
      int index = initial_msg_index(payloads, nb_rx);
      if (index != -1) {
        start_guti = gutis[index];
      }
    }

    if (traffic_status == DECREASED_LOAD && end_guti == MAX_SIZE){
      int index = initial_msg_index(payloads, nb_rx);
      if (index != -1) {
        end_guti = gutis[index];
      }
    }

    for (int i = 0; i < nb_rx; i++){
      ptr = rte_pktmbuf_mtod_offset(pkt_burst[i], uint8_t *, FLAG_OFFSET);
      if (start_guti == 0 || gutis[i] < start_guti || gutis[i] > end_guti){
        *ptr = LOCAL_CPF_PORT;
      } else{
        *ptr = REMOTE_CPF_PORT;
      }
    }

    // Forwarding message burst to workers through their ring buffers.
    ret = rte_ring_enqueue_burst(rings[index], pkt_burst, nb_rx, NULL);
    if (unlikely(ret < nb_rx))
    {
      drop_count += nb_rx - ret;
      while (ret != nb_rx)
      {
        rte_pktmbuf_free(pkt_burst[ret]);
        ret++;
      }
    }

    index = (index + 1) % RX_DEQUEUE_THREADS;
    update_rx(RX_PORT, nb_rx);
    count += nb_rx;
    
  }

  update_stats();

  // ===================================================
  printf("Received count at main RX: %u\n", count);
  printf("Pkts droped at RX: %u\n", drop_count);
  double total_time = (double)(c_end - c_start) / hz;
  double rx_rate = (rx_bytes * 8 * 1e-9) / total_time;
  printf("Rx Rate: %0.4f Gbps\n", rx_rate);
}

static void lock_cpf()
{
  FILE *fptr;
  fptr = fopen(LOCK_FILE, "w");
  fclose(fptr);
}

static void unlock_cpf() { remove(LOCK_FILE); }

static unsigned int get_next_lcore(int sock_id)
{
  curr_cores[sock_id] += CPU_SOCKETS;
  printf("Used cores: %d\n", curr_cores[sock_id]);
  return curr_cores[sock_id];
}

static void create_CPF(struct cpf_args *args)
{

  CPF *new_cpf = new CPF((uint16_t)(args->id), NULL, NULL,
                         args->id + 9091, args->sync_time, args->cpf_config,
                         args->cpu_loads, args->type);
  new_cpf->setID((uint8_t) args->id);
  if (args->type == 0){
    nicInterface.controller->add_l1_cpf(new_cpf);
  }
  nicInterface.controller->add_l2_cpf(new_cpf);

}

int main(int argc, char **argv)
{

  struct cpf_args cpf_args;

  InitLogger();
  vector<float> cpu_loads;
  vector<float> lb_cpu_loads;

  Config cfg;
  cfg.readFile("cta_config.cfg");

  // Read the configuration file
  int serializer = cfg.lookup("serializer");
  int number_of_cpf = cfg.lookup("number_of_cpf");
  int georep_experiment = cfg.lookup("georep_experiment");
  pkt_logging = cfg.lookup("pkt_logging");
  Setting &local_cpfs = cfg.lookup("local_cpfs");
  Setting &remote_cpfs = cfg.lookup("remote_cpfs");
  

  struct CTAConfig *conf = malloc(sizeof(CTAConfig));
  conf->serializer = serializer;
  conf->number_of_cpfs = number_of_cpf;
  conf->cpu_loads = &lb_cpu_loads;
  conf->georep_experiment = georep_experiment;

  setup_cpf(argc, argv);
  printf("===================================================\n");
  nicInterface.Init();
  unsigned ring_size = MAX_RING_SIZE / 4;

  for (unsigned int i = 0; i < CPU_SOCKETS; i++)
  {
    curr_cores[i] = i;
  }

  // Spawn a thread for memory logging
  if (pkt_logging != 0)
  {
    struct rte_ring *store_ring = rte_ring_create(
        "STORE_RING", STORE_RING_SIZE, rte_socket_id(), RING_F_SC_DEQ);
    rte_eal_remote_launch(init_store, store_ring,
                          get_next_lcore(rte_socket_id()));
  }

  // Spawns dequeue threads
  for (int i = 0; i < RX_DEQUEUE_THREADS; i++)
  {
    char ring_name[32];
    sprintf(ring_name, "Dequeue_%d", i);
    rings[i] = rte_ring_create(ring_name, ring_size, rte_socket_id(),
                               RING_F_SP_ENQ | RING_F_SC_DEQ);

    struct LBInfo *info = (struct LBInfo *)malloc(sizeof(LBInfo));
    info->id = i;
    info->ring = rings[i];
    info->config = conf;

    rte_eal_remote_launch(processPkt, info,
                          get_next_lcore(rte_socket_id()));
  }


  struct rte_ring **cpf_tx_rings = rte_malloc(NULL, sizeof(rte_ring*), 0);

  cpf_args.tx_rings = NULL;
  cpf_args.cpf_config = conf;

  // Create Local CPFs
   int id;
  for (int i = 0; i < local_cpfs.getLength(); i++)
  {
    local_cpfs[i].lookupValue("cpf", id);
    cpf_args.id = id - 1;
    cpf_args.type = 0;
    create_CPF(&cpf_args);
  }

  // Create Remote CPFs
  for (int i = 0; i < remote_cpfs.getLength(); i++)
  {
    remote_cpfs[i].lookupValue("cpf", id);
    cpf_args.id = id - 1;
    cpf_args.type = 1;
    create_CPF(&cpf_args);
  }

  nicInterface.setRxRings(NULL);

  // Spawn the threads that returns the messages back to the traffic generator.
  for (uint8_t i = 0; i < TX_DEQUEUE_THREADS; i++){

    char ring_name[32];
    sprintf(ring_name, "tx_ring_%d", i);
    cpf_tx_rings[i] = rte_ring_create(
        ring_name, CPF_TX_RING_SIZE, rte_socket_id(), RING_F_SC_DEQ);


    struct LBInfo *info = (struct LBInfo *) malloc(sizeof(LBInfo));
    info->id = i;
    info->config = conf;
    info->ring = cpf_tx_rings[i];

    rte_eal_remote_launch((lcore_function_t *) flush_tx, info, get_next_lcore(rte_socket_id()));
  }

  struct LBRtnInfo *rtn_info = (struct LBRtnInfo *) malloc(sizeof(LBRtnInfo));
  rtn_info->lb_rtn_rings = cpf_tx_rings;
  rtn_info->config = conf;

  // Spawn the thread that receives the messages from the CPFs.
  rte_eal_remote_launch((lcore_function_t *) listen_on_tx, rtn_info, get_next_lcore(rte_socket_id()));

  check_ports();
  lock_cpf();

  // Spawn the thread that receives the messages from the traffic generator.
  rte_eal_remote_launch((lcore_function_t *) recvPkts, conf, get_next_lcore(rte_socket_id()));

  uint16_t i;
  RTE_LCORE_FOREACH_SLAVE(i) {
    if (rte_eal_wait_lcore(i) < 0) {
      break;
    }
  }

  exit_cpf();
  unlock_cpf();
  free(conf);

  float sum = 0;
  for (float st: cpu_loads)
  {
      sum += st;
  }

  printf("Average CPU Load: %f\n", sum / cpu_loads.size());

  sum = 0;
  for (float st: lb_cpu_loads)
  {
      sum += st;
  }

  printf("Average LB CPU Load: %f\n", sum / lb_cpu_loads.size());

  CloseLogger();
  return 0;
}