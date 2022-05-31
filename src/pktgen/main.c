#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <rte_atomic.h>
#include <rte_branch_prediction.h>
#include <rte_common.h>
#include <rte_cycles.h>
#include <rte_debug.h>
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_ether.h>
#include <rte_interrupts.h>
#include <rte_launch.h>
#include <rte_lcore.h>
#include <rte_log.h>
#include <rte_malloc.h>
#include <rte_mbuf.h>
#include <rte_memcpy.h>
#include <rte_memory.h>
#include <rte_mempool.h>
#include <rte_per_lcore.h>
#include <rte_prefetch.h>
#include <rte_random.h>

#include "header.h"
#include "helper/globals.h"
#include "helper/init.h"
#include "helper/pkt_helper.h"
#include "state_machine.h"

#define WORKER_RING_SIZE (1 << 20)
// Number of mbufs to be used in creating the mempool for every worker
#define NB_W_MBUF (nb_txd * 3)

#define PKTGEN_STATS "pktgen_stats.txt"
#define LOCK_FILE "pktgen.lck"

typedef unsigned long long int nb_bytes_t;

// Only used for calculating the stats of every worker
struct worker_stats {
  uint64_t rx;
  uint64_t tx;
  uint64_t dropped;
  uint64_t new_proc;
  double tx_rate;
  double rx_rate;
  uint64_t start_tsc;
  uint64_t end_tsc;
  nb_bytes_t rx_bytes;
  nb_bytes_t tx_bytes;
} __rte_cache_aligned;
struct worker_stats worker_stats[MAX_NB_WORKERS];

// Only used in the internal working of the worker. The main shouldn't have access to this
typedef struct wrker_intrn {
  States init_state;
  size_t (*get_next_state)(States, uint8_t *);
  uint16_t (*decode)(uint8_t *, size_t, States *);
  guti_t guti_offset;
  guti_t guti;
  pcount_t proc_count;
  struct rte_mempool *mempool;
  uint64_t index;
  double tmp_buf[1 << 20];
} w_intern;

// Used to communicate configrations to the worker. Used by the main and consumed by worker
struct worker_conf {
  int id;
  struct rte_ring *rx_ring;
  int rx_port;
  int tx_port;
  int tx_qid;
  uint64_t offset;
  proc_type proc_type;
  bool georep_experiment;
  rate_t rate;
  struct worker_stats *stats;
  w_intern *intern;
} __rte_cache_aligned;
struct worker_conf worker_conf[MAX_NB_WORKERS];

struct rte_mempool *rx_mempool = NULL;

// TODO: This function should not be given stats because it does not know
// how many messages will be send. stats should be updated after the success
// function call to send_burst. For now, it works because send_burst is doing
// best effort.
// This function generates the first message of the procedure in bulk.
static inline uint16_t gen_new_burst(struct rte_mbuf **pkts,
                                     unsigned int burst_size, w_intern *intern,
                                     nb_bytes_t *tx_bytes) {
  unsigned int i;
  struct rte_mbuf *m;
  size_t payload_size;
  uint8_t *buff;
  int ret;

  ret = rte_pktmbuf_alloc_bulk(intern->mempool, pkts, burst_size);
  if (ret != 0) {
    // printf("No entries in mempool, droping a burst of %u responses\n");
    return 0;
  }

  for (i = 0; i < burst_size; i++) {
    m = pkts[i];
    buff = rte_pktmbuf_mtod_offset(m, uint8_t *, HEADER_SIZE);
    payload_size =
        intern->get_next_state(intern->init_state, buff + C_HEADER_SIZE);
    add_header(buff, intern->guti, payload_size);
    *tx_bytes += generate_packet(m, C_HEADER_SIZE + payload_size);
    intern->guti += intern->guti_offset;
  }
  return burst_size;
}

// Send the given burst of the message.
// Won't return unless all the messages are successfully sent (we call this best effort)
static inline uint16_t send_burst(uint16_t portid, uint16_t qid,
                                  struct rte_mbuf **pkts, uint16_t nb_pkts) {
  uint16_t to_send, sent;

  to_send = nb_pkts;
  while (to_send != 0) {
    sent = rte_eth_tx_burst(portid, qid, pkts + (nb_pkts - to_send), to_send);
    to_send -= sent;
  }
  return nb_pkts;
}

// Takes a single message, decodes it and send back the response, if any.

static inline uint16_t gen_response(uint8_t *pkt, struct rte_mbuf **rsp,
                                    w_intern *intern, nb_bytes_t *tx_bytes) {
  static int fst = 0;
  custom_header hdr;
  States nxt_states[3];
  struct rte_mbuf *m;
  size_t payload_size;
  uint8_t *buff;
  uint16_t nb_states, k;

  rte_memcpy(&hdr, pkt, C_HEADER_SIZE);
  // printf("Header size: %ld | Payload size: %ld\n", HEADER_SIZE, hdr.payload_length);

  // if (hdr.payload_length == 91){
  //   fst++;
  // }

  // printf("First: %d\n", fst);
  // printf("Payload length: %ld\n", hdr.payload_length);
  nb_states =
      intern->decode(pkt + C_HEADER_SIZE, hdr.payload_length, nxt_states);

  // printf("NB_STATES: %d\n", nb_states);

  // printf("NB_States: %d\n", nb_states);
  // This block is here only for fast handover
  if (nb_states == 1 && nxt_states[0] == DONE) {
    // log procedure completion time
    intern->tmp_buf[intern->index] = timestamp_micro() - hdr.timestamp;
    intern->index++;
    return 0;
  }

  for (k = 0; k < nb_states; k++) {
    m = rsp[k];
    buff = rte_pktmbuf_mtod_offset(m, uint8_t *, HEADER_SIZE);
    payload_size = intern->get_next_state(nxt_states[k], buff + C_HEADER_SIZE);
    update_header((uint8_t *)&hdr, buff, payload_size);

    if (nxt_states[k] == ATTACH_COMPLETE || nxt_states[k] == UE_CONTEXT_RELEASE_REQUEST ||
        nxt_states[k] == E_RAB_MODIFICATION_RESPONSE){
        hdr.last_msg = 1;
    }

    *tx_bytes += generate_packet(m, C_HEADER_SIZE + payload_size);

    // uint8_t *pk = rte_pktmbuf_mtod(m, uint8_t *);

    // for (int i = 0; i < HEADER_SIZE + C_HEADER_SIZE + payload_size; i++){
    //   printf("%d ", *(pk + i));
    // }
    // printf("\n\n");
  }
  return k;
}

// Take a burst of messages coming from cta, decodes every message and
// prepare the responses. Send back a burst of responses.
static inline uint16_t gen_burst_responses(
    struct rte_mbuf **pkts, unsigned int nb_pkts, struct rte_mbuf **rsp,
    w_intern *intern, nb_bytes_t *rx_bytes, nb_bytes_t *tx_bytes) {
  struct rte_mbuf *m;
  uint8_t *buff;
  uint16_t k;
  int ret;

  ret = rte_pktmbuf_alloc_bulk(intern->mempool, rsp, nb_pkts * 3);
  if (ret != 0) {
    printf("No entries in mempool, droping a burst of %d responses\n", nb_pkts);
    return 0;
  }
  k = 0;

  for (unsigned int i = 0; i < nb_pkts; i++) {
    m = pkts[i];

    // uint8_t *pk = rte_pktmbuf_mtod(m, uint8_t *);
    // for (int i = 0; i < HEADER_SIZE + 123; i++){
    //   printf("%d ", *(pk + i));
    // }
    // printf("\n\n");
#if PRINT_STATS
    *rx_bytes += m->pkt_len;
#endif
    buff = rte_pktmbuf_mtod_offset(m, uint8_t *, HEADER_SIZE);
    // TODO: We can reuse the same mbuf which is coming from rx mempool, it
    // might be a problem or it could result in a perfomance boost because we
    // don't have to reallocate memory nor do we have to call generate_packet
    // for setting the headers again.
    k += gen_response(buff, rsp + k, intern, tx_bytes);
    rte_pktmbuf_free(m);
  }

  // Freeing the unused mbufs here
  for (uint16_t i = k; i < nb_pkts * 3; i++) {
    rte_pktmbuf_free(rsp[i]);
  }
  return k;
}

#include <sys/time.h>

/* The worker thread is responsible for generating traffic in uniform mode
   It has two major responsibilities
   1. Checks for and processes any responses from the do_rx thread.
   2. Starts new procedures until all new procedures are finished.
 */
static int run_worker(void *args) {

  int rem_bursts = 1000;

  // 800 and 700 for handover
  // rem_bursts < 550 && rem_bursts > 450 local service
  int burst_start = 700;
  int burst_end = 800;

  struct worker_conf *conf = (struct worker_conf *)args;
  w_intern *intern = conf->intern;
  struct worker_stats *stats = conf->stats;
  struct rte_mbuf *pkts[MAX_PKT_BURST];
  struct rte_mbuf *rsp[MAX_PKT_BURST * 3];
  uint16_t nb_tx;
  unsigned int nb_rx;
  unsigned int burst_size = MAX_PKT_BURST;

#if PRINT_STATS
  unsigned int max_size = 0;
#endif

  // Calculate how much we have to wait in-between the burst
  double wait_per_burst = burst_size / (double)conf->rate;
  double wait_per_burst_decreased = burst_size / ((double) 8000);
  double current_burst = wait_per_burst;

  uint64_t start_ts = rte_get_timer_cycles();  

  uint64_t prev_ts = 0;
  uint64_t now_ts, end_ts;
  double elapsed;
  const uint64_t hz = rte_get_timer_hz();

  char name[64];
  sprintf(name, "worker_mempool_%d", conf->id);
  intern->mempool =
      rte_pktmbuf_pool_create(name, NB_W_MBUF, MEMPOOL_CACHE_SIZE, 0,
                              RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());

  printf("starting uniform worker %d on lcore %u with rate %u\n", conf->id,
         rte_lcore_id(), conf->rate);

  while (!force_quit) {
    // Read packet burst from RX ring, generate responses and send them back
    nb_rx = rte_ring_dequeue_burst(conf->rx_ring, (void **)pkts, MAX_PKT_BURST,
                                   NULL);
    if (nb_rx != 0) {
      stats->rx += nb_rx;
      nb_tx = gen_burst_responses(pkts, nb_rx, rsp, intern, &stats->rx_bytes,
                                  &stats->tx_bytes);
      stats->tx += send_burst(conf->tx_port, conf->tx_qid, rsp, nb_tx);
      stats->end_tsc = rte_get_timer_cycles();
    }
#if PRINT_STATS
    max_size = RTE_MAX(max_size, rte_ring_count(conf->rx_ring));
#endif

    // Start Procedures in uniform mode
    now_ts = rte_get_timer_cycles();

    elapsed = (double)(now_ts - prev_ts) / hz;
    if (elapsed >= current_burst && rem_bursts > 0) {
      if (unlikely(stats->tx == 0)) {
        stats->start_tsc = rte_get_timer_cycles();
      }
      nb_tx = gen_new_burst(pkts, burst_size, intern, &stats->tx_bytes);

      // Send the newly generated burst.
      stats->tx += send_burst(conf->tx_port, conf->tx_qid, pkts, nb_tx);
      prev_ts = now_ts;
      stats->end_tsc = rte_get_timer_cycles();

      if (conf->georep_experiment)
        rem_bursts--;
    }

    // Increase the burst sending frequency in a window.
    if (conf->georep_experiment){
      if (rem_bursts < burst_end && rem_bursts > burst_start && 
          (conf->proc_type == HANDOVER || conf->proc_type == SERVICE) ){
        current_burst = wait_per_burst_decreased;
      } else {
        current_burst = wait_per_burst;
      }
    }
    
  }

#if PRINT_STATS
  double total_time = (double)(stats->end_tsc - stats->start_tsc) / hz;
  stats->tx_rate = (stats->tx_bytes * 8 * 1e-9) / total_time;
  stats->rx_rate = (stats->rx_bytes * 8 * 1e-9) / total_time;
  printf("The capacity of the ring buffer is %u and the maximum size was %u\n",
         rte_ring_get_capacity(conf->rx_ring), max_size);
#endif
  return 0;
}

/* The worker thread is responsible for generating traffic in bursty mode
   It has two major responsibilities
   1. Checks for and processes any responses from the do_rx thread.
   2. Starts new procedures until all new procedures are finished.
 */

static int run_worker_bursty(void *args) {
  struct worker_conf *conf = (struct worker_conf *)args;
  w_intern *intern = conf->intern;
  struct worker_stats *stats = conf->stats;
  struct rte_mbuf *pkts[MAX_PKT_BURST];
  struct rte_mbuf *rsp[MAX_PKT_BURST * 3];
  uint16_t nb_tx;
  unsigned int nb_rx;
  unsigned int burst_size = MAX_PKT_BURST;

  int total = 0;

#if PRINT_STATS
  unsigned int max_size = 0;
#endif

  double wait_per_burst = burst_size / (double)conf->rate;
  uint64_t prev_ts = 0;
  uint64_t now_ts;
  double elapsed;
  const uint64_t hz = rte_get_timer_hz();

  char name[64];
  sprintf(name, "worker_mempool_%d", conf->id);
  intern->mempool =
      rte_pktmbuf_pool_create(name, NB_W_MBUF, MEMPOOL_CACHE_SIZE, 0,
                              RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());

  printf(
      "starting bursty worker %d on lcore %u with rate %u and proc_count %u\n",
      conf->id, rte_lcore_id(), conf->rate, conf->intern->proc_count);

  while (!force_quit) {
    // Read packet burst from RX ring, generate responses and send them back
    nb_rx = rte_ring_dequeue_burst(conf->rx_ring, (void **)pkts, MAX_PKT_BURST,
                                   NULL);

    if (nb_rx != 0) {

      stats->rx += nb_rx;
      nb_tx = gen_burst_responses(pkts, nb_rx, rsp, intern, &stats->rx_bytes,
                                  &stats->tx_bytes);
      stats->tx += send_burst(conf->tx_port, conf->tx_qid, rsp, nb_tx);
      total += nb_rx;
      stats->end_tsc = rte_get_timer_cycles();
    }
#if PRINT_STATS
    max_size = RTE_MAX(max_size, rte_ring_count(conf->rx_ring));
#endif

    // Start Procedures in bursty mode
    if (conf->intern->proc_count != 0) {
      now_ts = rte_get_timer_cycles();
      elapsed = (double)(now_ts - prev_ts) / hz;
      if (elapsed >= wait_per_burst) {
        if (unlikely(stats->tx == 0)) {
          stats->start_tsc = rte_get_timer_cycles();
        }
        burst_size = RTE_MIN(conf->intern->proc_count, MAX_PKT_BURST);
        nb_tx = gen_new_burst(pkts, burst_size, intern, &stats->tx_bytes);

        // Send the newly generated burst.
        stats->tx += send_burst(conf->tx_port, conf->tx_qid, pkts, nb_tx);
        total += nb_tx;
        prev_ts = now_ts;
        stats->end_tsc = rte_get_timer_cycles();
        conf->intern->proc_count -= nb_tx;
      }
    }

  }

#if PRINT_STATS
  double total_time = (double)(stats->end_tsc - stats->start_tsc) / hz;
  stats->tx_rate = (stats->tx_bytes * 8 * 1e-9) / total_time;
  stats->rx_rate = (stats->rx_bytes * 8 * 1e-9) / total_time;
  printf("The capacity of the ring buffer is %u and the maximum size was %u\n",
         rte_ring_get_capacity(conf->rx_ring), max_size);
#endif

  return 0;
}

/* A seprate thread is reserved for receiving the messages from CTA.
 Consumes RX and send the messages to workers using round robin scheduling through
 their individual ring buffers. 
*/
static int do_rx(void *args) {
  unsigned int nb_workers = *(unsigned int *)args;
  uint64_t index;
  struct rte_mbuf *pkts_burst[MAX_PKT_BURST];
  unsigned lcore_id;
  struct rte_ring *ring;
  uint16_t nb_rx;
  unsigned int ret;
#if PRINT_STATS
  uint64_t drop_count = 0;
#endif

  lcore_id = rte_lcore_id();
  index = 0;
  int total = 0;
  printf("Starting master thread for RX on lcore %u\n", lcore_id);
  while (!force_quit) {

    // RX Consumption
    nb_rx = rte_eth_rx_burst((uint8_t)RX_PORT, 0, pkts_burst, MAX_PKT_BURST);
    if (nb_rx == 0) {
      continue;
    }

    total += nb_rx;

    ring = worker_conf[index].rx_ring;

    // Forwarding message burst to workers through their ring buffers.
    ret = rte_ring_enqueue_burst(ring, (void *)pkts_burst, nb_rx, NULL);
    if (unlikely(ret < nb_rx)) {
#if PRINT_STATS
      drop_count += nb_rx - ret;
#endif
      while (ret != nb_rx) {
        rte_pktmbuf_free(pkts_burst[ret]);
        ret++;
      }
    }
    index = (index + 1) % nb_workers;
  }
#if PRINT_STATS
  printf("Pkts droped at RX: %lu\n", drop_count);
#endif
  printf("Total count received: %d\n", total);
  return 0;
}

// Used to set some variables for worker to use.
// More specifically used to set the function pointers to appropriate
// functions considering the procedure type.
static void init_intern(w_intern *intern, proc_type proc_type,
                        serial_type serial_type) {
  switch (proc_type) {
    case ATTACH:
      intern->init_state = ATTACH_REQUEST;
      if (serial_type == ASN1) {
        intern->decode = asn1_attach_decode;
        intern->get_next_state = asn1_attach_next_msg;
      } else if (serial_type == FB) {
        intern->decode = fb_attach_decode;
        intern->get_next_state = fb_attach_next_msg;
      }
      break;

    case HANDOVER:
      intern->init_state = HANDOVER_REQUIRED;
      if (serial_type == ASN1) {
        intern->decode = asn1_handovr_decode;
        intern->get_next_state = asn1_handovr_next_msg;
      } else if (serial_type == FB) {
        intern->decode = fb_handovr_decode;
        intern->get_next_state = fb_handovr_next_msg;
      }
      break;
    case SERVICE:
      intern->init_state = SERVICE_REQUEST;
      if (serial_type == ASN1) {
        intern->decode = asn1_srvc_req_decode;
        intern->get_next_state = asn1_srvc_req_next_msg;
      } else if (serial_type == FB) {
        intern->decode = fb_srvc_req_decode;
        intern->get_next_state = fb_srvc_req_next_msg;
      }
      break;
    case DPCM_SERVICE:
      intern->init_state = DPCM_SERVICE_REQUEST;
      if (serial_type == ASN1) {
        intern->decode = asn1_dpcm_srvc_decode;
        intern->get_next_state = asn1_dpcm_srvc_next_msg;
      }
      break;
    case FAST_HANDOVER:
      intern->init_state = HANDOVER_REQUEST_ACK;
      if (serial_type == FB) {
        intern->decode = fb_fast_ho_decode;
        intern->get_next_state = fb_fast_ho_next_msg;
      }
      break;
    default:
      break;
  }
}

// Take all these paramenters and build a worker_conf structure for a worker to consume.
static void config_worker(unsigned int id, int qid, guti_t guti_offset,
                          rate_t rate, proc_type proc_type,
                          serial_type serial_type, bool georep_experiment, pcount_t proc_count) {
  struct worker_conf *w = &worker_conf[id];

  w->id = id;
  char ring_name[64];
  sprintf(ring_name, "Worker_rx_ring_%d", id);
  w->rx_ring = rte_ring_create(ring_name, WORKER_RING_SIZE, rte_socket_id(),
                               RING_F_SC_DEQ);
  w->rx_port = RX_PORT;
  w->tx_port = TX_PORT;
  w->tx_qid = qid;
  w->rate = rate;
  w->proc_type = proc_type;
  w->georep_experiment = georep_experiment;

  w->stats = &worker_stats[id];
  memset(&worker_stats[id], 0, sizeof(struct worker_stats));
  w->intern = malloc(sizeof(w_intern));
  memset(w->intern, 0, sizeof(w_intern));
  w->intern->guti = id;
  w->intern->guti_offset = guti_offset;
  w->intern->proc_count = proc_count;
  init_intern(w->intern, proc_type, serial_type);
}

static void signal_handler(int signum) {
  if (signum == SIGINT || signum == SIGTERM) {
    printf("\n\nSignal %d received, preparing to exit...\n", signum);
    force_quit = true;
  }
}

static void close_port(uint16_t portid) {
  printf("Closing port %d...", portid);
  rte_eth_dev_stop(portid);
  rte_eth_dev_close(portid);
  printf(" Done\n");
}

// Let DPDK set the RX and TX port.
static void setup_ports(unsigned int nb_workers) {
  uint16_t nb_ports = rte_eth_dev_count();
  if (nb_ports < 2)
    rte_exit(EXIT_FAILURE, "We need atleast 2 ports to work.\n");

  rx_mempool = setup_rx_port();
  setup_tx_port(nb_workers);

  check_port_status(RX_PORT);
  check_port_status(TX_PORT);

  return;
}

// Used to get the next lcore id.
// We are doing +2 because we had two CPU sockets an we only wanted to use
// one to improve cache hit. Should be changed according the system architecture.
static void get_next_lcore(unsigned int *curr_lcore) { *curr_lcore += 2; }

static void lock_pktgen() {
  FILE *fptr;
  fptr = fopen(LOCK_FILE, "w");
  fclose(fptr);
}

static void unlock_pktgen() { remove(LOCK_FILE); }

// Writes the pktgen rate and other calculations to a file
static void write_stats(struct master_conf *conf) {
  char dname[64] = "logs/";
  struct stat st = {0};
  if (stat(dname, &st) == -1) {
    mkdir(dname, 0755);
  }

  strcat(dname, PKTGEN_STATS);

  FILE *fptr;
  fptr = fopen(dname, "w");

  double rx_rate = 0, tx_rate = 0, pkt_per_sec = 0;
  uint64_t rx = 0, tx = 0, dropped = 0;

  for (uint16_t i = 0; i < conf->u_cores; i++) {
    rx_rate += worker_conf[i].stats->rx_rate;
    tx_rate += worker_conf[i].stats->tx_rate;
    rx += worker_conf[i].stats->rx;
    tx += worker_conf[i].stats->tx;
    dropped += worker_conf[i].stats->dropped;
  }
  pkt_per_sec = (double)tx / conf->duration;

  fprintf(fptr, "Total Tx rate:          %0.2f Gbps\n", tx_rate);
  fprintf(fptr, "Total Rx rate:          %0.2f Gbps\n", rx_rate);
  fprintf(fptr, "Total Sent Packets:     %lu\n", tx);
  fprintf(fptr, "Total Received Packets: %lu\n", rx);
  fprintf(fptr, "Total Dropped Packets:  %lu\n", dropped);
  fprintf(fptr, "Packets per Sec:        %0.2f\n", pkt_per_sec);

  fclose(fptr);
}

// Writes PCT in the log file.
static void write_PCT(struct master_conf *conf) {
  char fname[64] = "logs/PCT.txt";
  FILE *fptr;
  fptr = fopen(fname, "w");

  uint64_t count = 0;

  for (uint16_t i = 0; i < conf->u_cores; i++) {
    count = worker_conf[i].intern->index;
    for (uint64_t j = 0; j < count; j++) {
      fprintf(fptr, "%.1f\n", worker_conf[i].intern->tmp_buf[j]);
    }
  }
  fclose(fptr);
}

int main(int argc, char **argv) {
  int ret;
  uint16_t nb_ports;
  uint16_t i;
  unsigned int nb_workers;
  unsigned int curr_core;
  unsigned int proc_share;
  struct master_conf *conf;

  conf = (struct master_conf *)malloc(sizeof(struct master_conf));

  /* init EAL */
  ret = rte_eal_init(argc, argv);
  if (ret < 0) rte_exit(EXIT_FAILURE, "Invalid EAL arguments\n");
  // printf("Hello");
  argc -= ret;
  argv += ret;

  /* parse application arguments (after the EAL ones) */
  ret = parse_args(argc, argv, conf);
  if (ret < 0) rte_exit(EXIT_FAILURE, "Invalid pktgen arguments\n");

  force_quit = false;
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);


  // Spawn an RX thread
  nb_workers = conf->u_cores;
  printf("Cores: %u\n", nb_workers);
  curr_core = rte_lcore_id();
  setup_ports(nb_workers);
  get_next_lcore(&curr_core);
  rte_eal_remote_launch(do_rx, (void *)&nb_workers, curr_core);

  // Check if uniform or burst and spawn the worker thread correspondingly
  printf("Proc type: %d", conf->proc_type);

  if (conf->proc_count > 0) {
    printf("Starting Bursty Workers\n");
    for (i = 0; i < conf->u_cores; i++) {
      proc_share = (conf->proc_count / (conf->u_cores - i));
      config_worker(i, i, nb_workers, conf->rate, conf->proc_type,
                    conf->serial_type, conf->georep_experiment, proc_share);
      get_next_lcore(&curr_core);
      rte_eal_remote_launch(run_worker_bursty, (void *)&worker_conf[i],
                            curr_core);
      conf->proc_count -= proc_share;
    }
  } else {
    printf("Starting Uniform Workers\n");
    for (i = 0; i < conf->u_cores; i++) {
      config_worker(i, i, nb_workers, conf->rate, conf->proc_type,
                    conf->serial_type, conf->georep_experiment, 0);
      get_next_lcore(&curr_core);
      rte_eal_remote_launch(run_worker, (void *)&worker_conf[i], curr_core);
    }
  }

  lock_pktgen();
  rte_delay_us_block(conf->duration * US_PER_S);
  printf("Stopping All worker...");
  force_quit = true;

  ret = 0;
  /* launch per-lcore init on every lcore */
  RTE_LCORE_FOREACH_SLAVE(i) {
    if (rte_eal_wait_lcore(i) < 0) {
      ret = -1;
      break;
    }
  }

  close_port(TX_PORT);
  close_port(RX_PORT);
#if PRINT_STATS
  write_stats(conf);
  if (conf->proc_type == FAST_HANDOVER) {
    write_PCT(conf);
  }
#endif
  free(conf);
  unlock_pktgen();
  printf("Bye...\n");
  return ret;
}
