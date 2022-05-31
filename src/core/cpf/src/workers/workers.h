#ifndef workers_h_
#define workers_h_

#include "../../Common/custom_header/CustomHeaderParser.h"
#include "../../Common/request/request.h"
#include "../map/map.h"
#include "../tools/state.h"


class DataWorker {
 private:
  CircularMap <guti_t, State *> &state_map;
  CustomHeaderParser headerParser;
  struct rte_mempool* rep_mempool;
  struct rte_ring *rx_ring;
  struct rte_ring *rep_ring;
    struct rte_ring *rep_ring_recv;
  struct CPFConfig *config;
  vector<map<unsigned long int, double>> *pcts;
  vector<float> *cpu_loads;
  vector<int> replica_ids;
  uint16_t id;
  int propagation_delay;

  inline uint16_t enqueue_bulk(State **states, uint16_t n);
  inline uint16_t replicate_state(State *s);

 public:
  DataWorker(uint16_t id, struct rte_ring *rx_ring, struct rte_ring *rep_ring, struct rte_ring *rep_ring_recv,  
             CircularMap<guti_t, State *> &state_map,
             struct CPFConfig *config, vector<map<unsigned long int, double>> *pcts,
             vector<float> *cpu_loads, vector<int> replica_ids, int propagation_delay);
  ~DataWorker();

  void setRepMempool(struct rte_mempool *rep_mempool);
  void run();
  void runRepRecv();
};

#endif
