#ifndef __cpf_h__
#define __cpf_h__


#include "../Common/globals.h"
#include "map/map.h"
#include "tools/state.h"
#include "workers/workers.h"

void runDataWorker(void *);
void runCPFSender(void *);
void runCPFReceiver(void *);

class CPF {
 private:
  DataWorker *worker;
  struct rte_ring *rx_ring;
  struct rte_ring *rep_ring;
  struct rte_ring *rep_ring_recv;
  struct rte_mempool *rep_mempool;
  struct CPFConfig *config;
  vector<map<unsigned long int, double>> *pcts;
  vector<float> *cpu_loads;
  vector<int> replica_ids;
  int hash;
  int propagation_delay;

  // Used only for debugging.
  unsigned int rcvd_count;
  unsigned int droped_count;
  CircularMap<unsigned long int, State *>
      state_map;  // Map to keep states of GUTI.

 public:
  CPF(uint16_t id, struct rte_ring *rx_ring, struct rte_ring *rx_ring_recv, 
      int cpf_port, struct CPFConfig *config,
      vector<map<unsigned long int, double>> *pcts, vector<float> *cpu_loads,
      vector<int> replica_ids, int propagation_delay);
  ~CPF();

  void start();
  void setRepRing(struct rte_ring *rep_ring);
  void setRepMempool(struct rte_mempool *rep_mempool);
  void syncReplicas();
  void startRecv();
  void createDataWorker();

  uint16_t id;
};

#endif
