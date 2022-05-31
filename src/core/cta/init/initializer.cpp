// DPDK header files.
#include <rte_common.h>
#include <rte_log.h>
#include <rte_malloc.h>
#include <rte_memory.h>
#include <rte_memcpy.h>
#include <rte_memzone.h>
#include <rte_eal.h>
#include <rte_per_lcore.h>
#include <rte_launch.h>
#include <rte_atomic.h>
#include <rte_cycles.h>
#include <rte_prefetch.h>
#include <rte_lcore.h>
#include <rte_per_lcore.h>
#include <rte_branch_prediction.h>
#include <rte_interrupts.h>
#include <rte_pci.h>
#include <rte_random.h>
#include <rte_debug.h>
#include <rte_ether.h>
#include <rte_ethdev.h>
#include <rte_mbuf.h>
#include <rte_ip.h>
#include <rte_udp.h>
#include <rte_tcp.h>
#include <rte_ring.h>

#include <getopt.h>
#include <signal.h>

#include "initializer.hpp"
#include "../Common/globals.h"

#define MAX_RX_QUEUE_PER_LCORE 16
#define MAX_TX_QUEUE_PER_PORT 16

/* MAC updating enabled by default */
static int mac_updating = 1;

static unsigned int l2fwd_rx_queue_per_lcore = 1;

#define MAX_TIMER_PERIOD 86400	/* 1 day max */
static uint64_t timer_period = 1; /* default period is 10 seconds */

static struct rte_eth_conf port_conf;

static const char short_options[] =
	"p:" /* portmask */
	"q:" /* number of queues */
	"T:" /* timer period */
	;

/* mask of enabled ports */
static uint32_t l2fwd_enabled_port_mask = 0;

#define CMD_LINE_OPT_MAC_UPDATING "mac-updating"
#define CMD_LINE_OPT_NO_MAC_UPDATING "no-mac-updating"

static const struct option lgopts[] = {
	{CMD_LINE_OPT_MAC_UPDATING, no_argument, &mac_updating, 1},
	{CMD_LINE_OPT_NO_MAC_UPDATING, no_argument, &mac_updating, 0},
	{NULL, 0, 0, 0}};

struct lcore_queue_conf
{
	unsigned n_rx_port;
	unsigned rx_port_list[MAX_RX_QUEUE_PER_LCORE];
} __rte_cache_aligned;
struct lcore_queue_conf lcore_queue_conf[RTE_MAX_LCORE];

/* Per-port statistics struct */
struct l2fwd_port_statistics
{
	uint64_t tx;
	uint64_t rx;
	uint64_t dropped;
} __rte_cache_aligned;
struct l2fwd_port_statistics port_statistics[RTE_MAX_ETHPORTS];

/* list of enabled ports */
static uint32_t l2fwd_dst_ports[RTE_MAX_ETHPORTS];

/* ethernet addresses of ports */
static struct ether_addr l2fwd_ports_eth_addr[RTE_MAX_ETHPORTS];

#define RTE_TEST_RX_DESC_DEFAULT (1 << 12)
#define RTE_TEST_TX_DESC_DEFAULT (1 << 12)

static uint16_t nb_rxd = RTE_TEST_RX_DESC_DEFAULT;
static uint16_t nb_txd = RTE_TEST_TX_DESC_DEFAULT;

// #define NB_MBUF ((1 << 23) - 1) # not for pcpf and pktgen servers
#define NB_MBUF ((1 << 21) - 1)
static uint64_t nb_mbuf = NB_MBUF;
#define MEMPOOL_CACHE_SIZE 512

uint8_t nb_ports;
unsigned lcore_id;

extern struct rte_mempool *rx_tx_mempool = NULL;
struct rte_eth_dev_tx_buffer *tx_buffer[RTE_MAX_ETHPORTS] = {};

#define BURST_TX_DRAIN_US 100 /* TX drain every ~100us */

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

/* Check the link status of all ports in up to 9s, and print them finally */
static void
check_all_ports_link_status(uint8_t port_num, uint32_t port_mask)
{
#define CHECK_INTERVAL 100 /* 100ms */
#define MAX_CHECK_TIME 90  /* 9s (90 * 100ms) in total */
	uint8_t portid, count, all_ports_up, print_flag = 0;
	struct rte_eth_link link;

	printf("\nChecking link status");
	fflush(stdout);
	for (count = 0; count <= MAX_CHECK_TIME; count++)
	{
		if (force_quit)
			return;
		all_ports_up = 1;
		for (portid = 0; portid < port_num; portid++)
		{
			if (force_quit)
				return;
			if ((port_mask & (1 << portid)) == 0)
				continue;
			memset(&link, 0, sizeof(link));
			rte_eth_link_get_nowait(portid, &link);
			/* print link status if flag set */
			if (print_flag == 1)
			{
				if (link.link_status)
					printf("Port %d Link Up - speed %u "
						   "Mbps - %s\n",
						   (uint8_t)portid,
						   (unsigned)link.link_speed,
						   (link.link_duplex == ETH_LINK_FULL_DUPLEX) ? ("full-duplex") : ("half-duplex\n"));
				else
					printf("Port %d Link Down\n",
						   (uint8_t)portid);
				continue;
			}
			/* clear all_ports_up flag if any link down */
			if (link.link_status == ETH_LINK_DOWN)
			{
				all_ports_up = 0;
				break;
			}
		}
		/* after finally printing all link status, get out */
		if (print_flag == 1)
			break;

		if (all_ports_up == 0)
		{
			printf(".");
			fflush(stdout);
			rte_delay_ms(CHECK_INTERVAL);
		}

		/* set the print_flag if all ports up or timeout */
		if (all_ports_up == 1 || count == (MAX_CHECK_TIME - 1))
		{
			print_flag = 1;
			printf("done\n");
		}
	}
}

static int
l2fwd_parse_portmask(const char *portmask)
{
	char *end = NULL;
	unsigned long pm;

	/* parse hexadecimal string */
	pm = strtoul(portmask, &end, 16);
	if ((portmask[0] == '\0') || (end == NULL) || (*end != '\0'))
		return -1;

	if (pm == 0)
		return -1;

	return pm;
}

static unsigned int
l2fwd_parse_nqueue(const char *q_arg)
{
	char *end = NULL;
	unsigned long n;

	/* parse hexadecimal string */
	n = strtoul(q_arg, &end, 10);
	if ((q_arg[0] == '\0') || (end == NULL) || (*end != '\0'))
		return 0;
	if (n == 0)
		return 0;
	if (n >= MAX_RX_QUEUE_PER_LCORE)
		return 0;

	return n;
}

static int
l2fwd_parse_timer_period(const char *q_arg)
{
	char *end = NULL;
	int n;

	/* parse number string */
	n = strtol(q_arg, &end, 10);
	if ((q_arg[0] == '\0') || (end == NULL) || (*end != '\0'))
		return -1;
	if (n >= MAX_TIMER_PERIOD)
		return -1;

	return n;
}

/* Print out statistics on packets dropped */
static void
print_stats(void)
{
	uint64_t total_packets_dropped, total_packets_tx, total_packets_rx;
	unsigned portid;

	total_packets_dropped = 0;
	total_packets_tx = 0;
	total_packets_rx = 0;

	const char clr[] = {27, '[', '2', 'J', '\0'};
	const char topLeft[] = {27, '[', '1', ';', '1', 'H', '\0'};

	/* Clear screen and move to top left */
	printf("%s%s", clr, topLeft);

	printf("\nPort statistics ====================================");

	for (portid = 0; portid < RTE_MAX_ETHPORTS; portid++)
	{
		/* skip disabled ports */
		if ((l2fwd_enabled_port_mask & (1 << portid)) == 0)
			continue;
		printf("\nStatistics for port %u ------------------------------"
			   "\nPackets sent: %24" PRIu64
			   "\nPackets received: %20" PRIu64
			   "\nPackets dropped: %21" PRIu64,
			   portid,
			   port_statistics[portid].tx,
			   port_statistics[portid].rx,
			   port_statistics[portid].dropped);

		total_packets_dropped += port_statistics[portid].dropped;
		total_packets_tx += port_statistics[portid].tx;
		total_packets_rx += port_statistics[portid].rx;
	}
	printf("\nAggregate statistics ==============================="
		   "\nTotal packets sent: %18" PRIu64
		   "\nTotal packets received: %14" PRIu64
		   "\nTotal packets dropped: %15" PRIu64,
		   total_packets_tx,
		   total_packets_rx,
		   total_packets_dropped);
	printf("\n====================================================\n");
}

/* display usage */
static void
l2fwd_usage(const char *prgname)
{
	printf("%s [EAL options] -- -p PORTMASK [-q NQ]\n"
		   "  -p PORTMASK: hexadecimal bitmask of ports to configure\n"
		   "  -q NQ: number of queue (=ports) per lcore (default is 1)\n"
		   "  -T PERIOD: statistics will be refreshed each PERIOD seconds (0 to disable, 10 default, 86400 maximum)\n"
		   "  --[no-]mac-updating: Enable or disable MAC addresses updating (enabled by default)\n"
		   "      When enabled:\n"
		   "       - The source MAC address is replaced by the TX port MAC address\n"
		   "       - The destination MAC address is replaced by 02:00:00:00:00:TX_PORT_ID\n",
		   prgname);
}

static int
l2fwd_parse_args(int argc, char **argv)
{
	int opt, ret, timer_secs;
	char **argvopt;
	int option_index;
	char *prgname = argv[0];

	argvopt = argv;

	while ((opt = getopt_long(argc, argvopt, short_options,
							  lgopts, &option_index)) != EOF)
	{

		switch (opt)
		{
		/* portmask */
		case 'p':
			l2fwd_enabled_port_mask = l2fwd_parse_portmask(optarg);
			if (l2fwd_enabled_port_mask == 0)
			{
				printf("invalid portmask\n");
				l2fwd_usage(prgname);
				return -1;
			}
			break;

		/* nqueue */
		case 'q':
			l2fwd_rx_queue_per_lcore = l2fwd_parse_nqueue(optarg);
			if (l2fwd_rx_queue_per_lcore == 0)
			{
				printf("invalid queue number\n");
				l2fwd_usage(prgname);
				return -1;
			}
			break;

		/* timer period */
		case 'T':
			timer_secs = l2fwd_parse_timer_period(optarg);
			if (timer_secs < 0)
			{
				printf("invalid timer period\n");
				l2fwd_usage(prgname);
				return -1;
			}
			timer_period = timer_secs;
			break;

		/* long options */
		case 0:
			break;

		default:
			l2fwd_usage(prgname);
			return -1;
		}
	}

	if (optind >= 0)
		argv[optind - 1] = prgname;

	ret = optind - 1;
	optind = 1; /* reset getopt lib */
	return ret;
}

void setup_cpf(int argc, char **argv)
{

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	port_conf.rxmode.split_hdr_size = 0;
	port_conf.rxmode.mq_mode = ETH_MQ_RX_RSS;
	// port_conf.rxmode.header_split = 0;
	// port_conf.rxmode.hw_ip_checksum = 0;
	// port_conf.rxmode.hw_vlan_filter = 0;
	port_conf.rxmode.jumbo_frame = 0;
	// port_conf.rxmode.hw_strip_crc = 0;
	port_conf.txmode.mq_mode = ETH_MQ_TX_NONE;

	struct lcore_queue_conf *qconf;
	struct rte_eth_dev_info dev_info;
	int ret;
	uint8_t nb_ports_available;
	uint8_t portid, last_port;
	unsigned rx_lcore_id;
	unsigned nb_ports_in_mask = 0;

	/* init EAL */
	ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Invalid EAL arguments\n");
	argc -= ret;
	argv += ret;

	/* parse application arguments (after the EAL ones) */
	ret = l2fwd_parse_args(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Invalid L2FWD arguments\n");

	/* convert to number of cycles */
	timer_period *= rte_get_timer_hz();

	nb_ports = rte_eth_dev_count();
	if (nb_ports == 0)
		rte_exit(EXIT_FAILURE, "No Ethernet ports - bye\n");

	/* reset l2fwd_dst_ports */
	for (portid = 0; portid < RTE_MAX_ETHPORTS; portid++)
		l2fwd_dst_ports[portid] = 0;
	last_port = 0;

	rx_lcore_id = 0;
	qconf = NULL;

	nb_ports_available = nb_ports;
	/* create the mbuf pool */
	rx_tx_mempool = rte_pktmbuf_pool_create("mbuf_pool", nb_mbuf,
											MEMPOOL_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE,
											rte_socket_id());
	if (rx_tx_mempool == NULL)
		rte_exit(EXIT_FAILURE, "Cannot init mbuf pool\n");

	printf("rte_mempool_avail_count: %u\n", rte_mempool_avail_count(rx_tx_mempool));

	/* Initialise each port */
	printf("nb_ports: %d\n", nb_ports);
	int queues = RTE_MAX(RX_DEQUEUE_THREADS, TX_DEQUEUE_THREADS); 
	printf("Queues: %d\n", queues);
	for (portid = 0; portid < nb_ports; portid++)
	{
		/* skip ports that are not enabled */
		if ((l2fwd_enabled_port_mask & (1 << portid)) == 0)
		{
			printf("Skipping disabled port %u\n", (unsigned)portid);
			nb_ports_available--;
			continue;
		}
		/* init port */
		printf("Initializing port %u... ", (unsigned)portid);
		fflush(stdout);
		ret = rte_eth_dev_configure(portid, queues, 
									queues+1, &port_conf);
		if (ret < 0)
			rte_exit(EXIT_FAILURE, "Cannot configure device: err=%d, port=%u\n",
					 ret, (unsigned)portid);

		rte_eth_macaddr_get(portid, &l2fwd_ports_eth_addr[portid]);

		/* init one RX queue */
		fflush(stdout);
		struct rte_eth_rxconf rxq_conf;
		rxq_conf = dev_info.default_rxconf;
  		rxq_conf.offloads = port_conf.rxmode.offloads;

		for (int i = 0; i < queues; i++){
			ret = rte_eth_rx_queue_setup(portid, i, nb_rxd,
												rte_eth_dev_socket_id(portid),
												&rxq_conf,
												rx_tx_mempool);
			if (ret < 0)
				rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup:err=%d, port=%u\n",
						ret, (unsigned)portid);
		}

		

		// ret = rte_eth_rx_queue_setup(portid, 1, nb_rxd,
		
		/* init one TX queue on each port */
		fflush(stdout);
		for (int i = 0; i < queues+1; i++) {
			ret = rte_eth_tx_queue_setup(portid, i, nb_txd,
									rte_eth_dev_socket_id(portid),
									NULL);
			if (ret < 0){
				rte_exit(EXIT_FAILURE, "rte_eth_tx_queue_setup:err=%d, port=%u\n", ret,
						portid);
				
			}
		}
		// }

		/* Start device */
		ret = rte_eth_dev_start(portid);
		if (ret < 0)
			rte_exit(EXIT_FAILURE, "rte_eth_dev_start:err=%d, port=%u\n",
					 ret, (unsigned)portid);

		printf("done: \n");

		rte_eth_promiscuous_enable(portid);

		printf("Port %u, MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n\n",
			   (unsigned)portid,
			   l2fwd_ports_eth_addr[portid].addr_bytes[0],
			   l2fwd_ports_eth_addr[portid].addr_bytes[1],
			   l2fwd_ports_eth_addr[portid].addr_bytes[2],
			   l2fwd_ports_eth_addr[portid].addr_bytes[3],
			   l2fwd_ports_eth_addr[portid].addr_bytes[4],
			   l2fwd_ports_eth_addr[portid].addr_bytes[5]);

		/* initialize port stats */
		memset(&port_statistics, 0, sizeof(port_statistics));
	}

	if (!nb_ports_available)
	{
		rte_exit(EXIT_FAILURE,
				 "All available ports are disabled. Please set portmask.\n");
	}

	global_mempool = rte_pktmbuf_pool_create("Global-mempool", NB_MBUF,
											 MEMPOOL_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE,
											 rte_socket_id());
	if (global_mempool == NULL)
		rte_exit(EXIT_FAILURE, "Cannot init mbuf pool\n");
}

void check_ports()
{
	check_all_ports_link_status(nb_ports, l2fwd_enabled_port_mask);
}

void exit_cpf()
{
	int ret = 0;
	RTE_LCORE_FOREACH_SLAVE(lcore_id)
	{
		if (rte_eal_wait_lcore(lcore_id) < 0)
		{
			ret = -1;
			break;
		}
	}

	for (unsigned portid = 0; portid < nb_ports; portid++)
	{
		if ((l2fwd_enabled_port_mask & (1 << portid)) == 0)
			continue;
		printf("Closing port %d...\n", portid);
		rte_eth_dev_stop(portid);
		rte_eth_dev_close(portid);
	}
	printf("Bye...\n");
}

void update_rx(unsigned port, uint16_t pkts)
{
	port_statistics[port].rx += pkts;
}

void update_tx(unsigned port, uint16_t pkts)
{
	port_statistics[port].tx += pkts;
}

static void update_stats(void)
{
	static uint64_t diff_tsc, cur_tsc;
	static uint64_t prev_tsc = 0;
	static uint64_t timer_tsc = 0;
	static const uint64_t drain_tsc = (rte_get_tsc_hz() + US_PER_S - 1) / US_PER_S *
									  BURST_TX_DRAIN_US;

	cur_tsc = rte_rdtsc();

	diff_tsc = cur_tsc - prev_tsc;
	if (unlikely(diff_tsc > drain_tsc))
	{
		/* if timer is enabled */
		if (timer_period > 0)
		{
			/* advance the timer */
			timer_tsc += diff_tsc;

			/* if timer has reached its timeout */
			if (unlikely(timer_tsc >= timer_period))
			{
				/* do this only on master core */
				if (lcore_id == rte_get_master_lcore())
				{
					print_stats();
					/* reset the timer */
					timer_tsc = 0;
				}
			}
		}
		prev_tsc = cur_tsc;
	}
}