#ifndef GLOBALS_H_
#define GLOBALS_H_

#define MAX_PKT_SIZE 1024
#define MAX_PKT_BURST 32
#define SYNC_BATCH_SIZE 32

// Ports configuration
#define PORT 3
#define REPLICATION_PORT 2
#define TOTAL_PORTS 2

#include <rte_mempool.h>
#include <vector>

extern volatile bool force_quit;

struct cpf_actions {
  int action;
  std::vector<int> config;
};

struct CPFConfig {
  int serializer;
  uint8_t replication;
  int number_of_cpfs;
  std::vector<float> *cpu_loads;
};

extern struct rte_mempool *global_mempool;

#endif
