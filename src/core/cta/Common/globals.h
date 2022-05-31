#ifndef GLOBALS_H_
#define GLOBALS_H_

#define MAX_PKT_SIZE 1024
#define MAX_PKT_BURST 32

// Ports configuration
#define REMOTE_CPF_PORT 0
#define LOCAL_CPF_PORT 1
#define RX_PORT 2
#define TX_PORT 3

#include <rte_mempool.h>
#include <rte_ethdev.h>
#include <rte_ip.h>
#include <rte_udp.h>

#include <vector>
#include "DataType.h"

const size_t FLAG_OFFSET =
    sizeof(double) + sizeof(lgclock_t) + sizeof(size_t) + sizeof(guti_t) + sizeof(uint8_t);
const size_t PAYLOAD_OFFSET =
    sizeof(struct ether_hdr) + sizeof(struct ipv4_hdr) + sizeof(struct udp_hdr);
const size_t PAYLOAD_LENGTH_OFFSET =
    PAYLOAD_OFFSET + sizeof(double) + sizeof(lgclock_t);
const size_t GUTI_OFFSET =
    PAYLOAD_LENGTH_OFFSET + sizeof(size_t);

union bytesToNumber {
  uint8_t buff[2];
  uint16_t value;
};

union logicalClockExtractor {
  uint8_t buff[8];
  unsigned long int logical_clock;
};

union payloadLenExtractor {
  uint8_t buff[8];
  size_t payload_length;
};

union gutiExtractor {
  uint8_t buff[8];
  guti_t guti;
};

struct cpf_actions {
  int action;
  std::vector<int> config;
};

struct CTAConfig {
  int serializer;
  int georep_experiment;
  int sync_time;
  uint8_t replication;
  int number_of_cpfs;
  int replicas;
  std::vector<float> *cpu_loads;
};

struct LBInfo {
  int id;
  struct rte_ring *ring;
  struct CTAConfig *config;
};

struct LBRtnInfo {

  struct rte_ring **lb_rtn_rings;
  struct CTAConfig *config;
};

enum traffic_status 
{   
    LOW_LOAD = 0, 
    HIGH_LOAD = 1, 
    DECREASED_LOAD = 2
};

extern struct rte_mempool *global_mempool;
extern int pkt_logging;
extern volatile bool force_quit;

const int RX_DEQUEUE_THREADS = 6;
const int TX_DEQUEUE_THREADS = 4;

#endif
