// This file parse the comand line arguments and setup the master_conf
// It is also used to setup RX and TX port.

#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <sys/time.h>
#include <sys/types.h>

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

#include "init.h"

const size_t HEADER_SIZE =
    sizeof(struct ether_hdr) + sizeof(struct ipv4_hdr) + sizeof(struct udp_hdr);

static struct rte_eth_conf port_conf = {
    .rxmode =
        {
            .split_hdr_size = 0,
        },
    .txmode =
        {
            .mq_mode = ETH_MQ_TX_NONE,
        },
};

/* display usage */
static void pktgen_usage(const char *prgname) {
  printf(
      "%s [EAL options] -- -t\n"
      "  -d Experiment Duration: experiment duration in seconds\n"
      "  -u Number of Uniform workers: workers which will be generating "
      "uniform traffic, default is %d\n"
      "  -s Serialization Scheme: serialization scheme to use. Possible values "
      "are asn1 and fb\n"
      "  -p Procedure Type: type of procedure to initiate on each worker. "
      "Possible values are attach, handover and service\n"
      "  -c Count of procedures: number of procedures to initiate\n"
      "  -r Uniform Rate: rate for each uniform workers, default is %d\n"
      "  -g Enable/Disable georeplication experiment",
      prgname, MAX_NB_WORKERS, DEFAULT_U_RATE);
}

static unsigned int pktgen_parse_duration(const char *t_arg) {
  char *end = NULL;
  unsigned long n;

  /* parse hexadecimal string */
  n = strtoul(t_arg, &end, 10);
  if ((t_arg[0] == '\0') || (end == NULL) || (*end != '\0')) return 0;
  if (n == 0) return 0;

  return n;
}

static unsigned int pktgen_parse_u_workers(const char *t_arg) {
  char *end = NULL;
  unsigned int n;

  n = strtol(t_arg, &end, 10);
  if ((t_arg[0] == '\0') || (end == NULL) || (*end != '\0')) return 0;
  if (n == 0) return 0;

  return n;
}

static rate_t pktgen_parse_rate(const char *t_arg) {
  char *end = NULL;
  unsigned int n;

  n = strtoul(t_arg, &end, 10);
  if ((t_arg[0] == '\0') || (end == NULL) || (*end != '\0')) return 0;
  if (n == 0) return 0;

  return n;
}

static pcount_t pktgen_parse_pcount(const char *c_arg) {
  char *end = NULL;
  unsigned int n;

  n = strtoul(c_arg, &end, 10);
  if ((c_arg[0] == '\0') || (end == NULL) || (*end != '\0')) return 0;
  if (n == 0) return 0;

  return n;
}

static proc_type pktgen_parse_proc(const char *t_arg) {
  if (strcmp(t_arg, "attach") == 0) {
    return ATTACH;
  } else if (strcmp(t_arg, "handover") == 0) {
    return HANDOVER;
  } else if (strcmp(t_arg, "service") == 0) {
    return SERVICE;
  } else if (strcmp(t_arg, "fast") == 0) {
    return FAST_HANDOVER;
  } else if (strcmp(t_arg, "dpcm_service") == 0) {
    return DPCM_SERVICE;
  }
  return -1;
}

static serial_type pktgen_parse_serial(const char *t_arg) {
  if (strcmp(t_arg, "asn1") == 0) {
    return ASN1;
  } else if (strcmp(t_arg, "fb") == 0) {
    return FB;
  }
  return -1;
}

static bool pktgen_parse_georep(const char *t_arg) {
  char *end = NULL;
  unsigned int n;

  n = strtoul(t_arg, &end, 10);
  if ((t_arg[0] == '\0') || (end == NULL) || (*end != '\0')) return 0;
  if (n == 0) return 0;

  return n;
}

static const char short_options[] =
    "d:" /* duration of the experiment in sec */
    "u:" /* number of uniform workers */
    "r:" /* rate for uniform workers */
    "p:" /* procedure type of each worker */
    "s:" /* serialization/deserialization to use */
    "c:" /* number of total procedures to start */
    "g:" /* Enable/Disable georeplication experiment */
    ;

/* Parse the argument given in the command line of the application */
int parse_args(int argc, char **argv, struct master_conf *conf) {
  int opt, ret;
  char **argvopt;
  int option_index;
  char *prgname = argv[0];

  argvopt = argv;

  while ((opt = getopt_long(argc, argvopt, short_options, NULL,
                            &option_index)) != EOF) {
    switch (opt) {
      case 'd':
        conf->duration = pktgen_parse_duration(optarg);
        if (conf->duration == 0) {
          printf("invalid time for experiment\n");
          pktgen_usage(prgname);
          return -1;
        }
        break;

      case 'c':
        conf->proc_count = pktgen_parse_pcount(optarg);
        break;

      case 'u':
        conf->u_cores = pktgen_parse_u_workers(optarg);
        if (conf->u_cores == 0) {
          conf->u_cores = MAX_NB_WORKERS;
        } else if (conf->u_cores > MAX_NB_WORKERS) {
          printf(
              "The number of workers (given %d) should be less than "
              "MAX_NB_WORKERS (%d).",
              conf->u_cores, MAX_NB_WORKERS);
          pktgen_usage(prgname);
          return -1;
        }
        break;

      case 'r':
        conf->rate = pktgen_parse_rate(optarg);
        if (conf->rate == 0) {
          conf->rate = DEFAULT_U_RATE;
        }
        break;

      case 'p':
        conf->proc_type = pktgen_parse_proc(optarg);
        if (conf->proc_type == -1) {
          pktgen_usage(prgname);
          return -1;
        }
        break;

      case 's':
        conf->serial_type = pktgen_parse_serial(optarg);
        if (conf->serial_type == -1) {
          pktgen_usage(prgname);
          return -1;
        }
        break;
      case 'g':
        conf->georep_experiment = pktgen_parse_georep(optarg);
        if (conf->georep_experiment != 0 && conf->georep_experiment != 1) {
          pktgen_usage(prgname);
          return -1;
        }
        break;
      /* long options */
      case 0:
        break;

      default:
        pktgen_usage(prgname);
        return -1;
    }
  }

  if (optind >= 0) argv[optind - 1] = prgname;

  ret = optind - 1;
  optind = 1; /* reset getopt lib */
  return ret;
}

struct rte_mempool *setup_rx_port(void) {
  uint16_t portid;
  unsigned int nb_mbufs;
  int ret;
  struct rte_eth_rxconf rxq_conf;
  struct rte_eth_conf local_port_conf = port_conf;
  struct rte_eth_dev_info dev_info;

  portid = RX_PORT;
  nb_mbufs = (1 << 22) - 1;
  struct rte_mempool *rx_mempool =
      rte_pktmbuf_pool_create("mbuf_pool", nb_mbufs, MEMPOOL_CACHE_SIZE, 0,
                              RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());
  if (rx_mempool == NULL) rte_exit(EXIT_FAILURE, "Cannot init mbuf pool\n");

  printf("Initializing RX port %u...\n", portid);
  fflush(stdout);

  // Support for multi segment packets.
  rte_eth_dev_info_get(portid, &dev_info);
  if (dev_info.tx_offload_capa & DEV_TX_OFFLOAD_MBUF_FAST_FREE)
    local_port_conf.txmode.offloads |= DEV_TX_OFFLOAD_MBUF_FAST_FREE;

  // Setup port for rx and tx queues
  ret = rte_eth_dev_configure(portid, 1, 0, &local_port_conf);
  if (ret < 0)
    rte_exit(EXIT_FAILURE, "Cannot configure device: err=%d, port=%u\n", ret,
             portid);

  /* init a single RX queue */
  rxq_conf = dev_info.default_rxconf;
  rxq_conf.offloads = local_port_conf.rxmode.offloads;
  ret = rte_eth_rx_queue_setup(portid, 0, nb_rxd, rte_eth_dev_socket_id(portid),
                               &rxq_conf, rx_mempool);
  if (ret < 0)
    rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup:err=%d, port=%u\n", ret,
             portid);

  /* Start device */
  ret = rte_eth_dev_start(portid);
  if (ret < 0)
    rte_exit(EXIT_FAILURE, "rte_eth_dev_start:err=%d, port=%u\n", ret, portid);

  rte_eth_promiscuous_enable(portid);

  printf("RX port %d is up!\n", portid);
  fflush(stdout);

  return rx_mempool;
}

void setup_tx_port(unsigned int nb_workers) {
  uint16_t portid;
  unsigned int i;
  int ret;
  struct rte_eth_txconf txq_conf;
  struct rte_eth_conf local_port_conf = port_conf;
  struct rte_eth_dev_info dev_info;

  portid = TX_PORT;
  printf("Initializing TX port %u...\n", portid);
  fflush(stdout);

  // Support for multi segment packets.
  rte_eth_dev_info_get(portid, &dev_info);
  if (dev_info.tx_offload_capa & DEV_TX_OFFLOAD_MBUF_FAST_FREE)
    local_port_conf.txmode.offloads |= DEV_TX_OFFLOAD_MBUF_FAST_FREE;

  // Setup port for rx and tx queues
  ret = rte_eth_dev_configure(portid, 0, nb_workers, &local_port_conf);
  if (ret < 0)
    rte_exit(EXIT_FAILURE, "Cannot configure device: err=%d, port=%u\n", ret,
             portid);

  /* init nb_workers TX queue(s) for each worker */
  txq_conf = dev_info.default_txconf;
  txq_conf.offloads = local_port_conf.txmode.offloads;
  for (i = 0; i < nb_workers; i++) {
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

  printf("TX port %d is up!\n", portid);
  fflush(stdout);
}

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