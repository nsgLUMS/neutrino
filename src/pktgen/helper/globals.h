#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <ctype.h>
#include <stdbool.h>

#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_udp.h>
#include "state_machine.h"

#define TX_PORT 2
#define RX_PORT 3

#define MAX_PKT_BURST 32
#define PRINT_STATS 1
#define MEMPOOL_CACHE_SIZE 512
#define DEFAULT_U_RATE 10000
#define MAX_NB_WORKERS 18

typedef unsigned int rate_t;
typedef unsigned int pcount_t;

// enum to enumrate all types of procedures
typedef enum proc_type {
  ATTACH,
  HANDOVER,
  SERVICE,
  FAST_HANDOVER,
  DPCM_SERVICE,
} proc_type;

// enum for all serialization schemes
typedef enum serial_type {
  FB,
  ASN1,
} serial_type;

struct master_conf {
  unsigned int duration;
  uint16_t u_cores;
  proc_type proc_type;
  serial_type serial_type;
  rate_t rate;
  pcount_t proc_count;
  bool georep_experiment;
};

extern const size_t HEADER_SIZE;

volatile bool force_quit;
// RX and TX descripters
static uint16_t nb_rxd = (1 << 12);
static uint16_t nb_txd = (1 << 12);

#endif
