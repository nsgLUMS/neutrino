#include <rte_ethdev.h>

#include "cpf.h"
#include "workers/workers.h"

using namespace std;

void runDataWorker(void *data) {
  CPF *cpf = (CPF *)data;
  printf("Starting an CPF on lcore %d\n", rte_lcore_id());
  cpf->start();
}

void runCPFSender(void *data) {
  CPF *cpf = (CPF *)data;
  printf("Starting CPF Sender thread on lcore %d\n", rte_lcore_id());
  cpf->syncReplicas();
}

void runCPFReceiver(void *data) {
  CPF *cpf = (CPF *)data;
  printf("Starting CPF Receiver thread on lcore %d\n", rte_lcore_id());
  cpf->startRecv();
}



CPF::CPF(uint16_t id, struct rte_ring *rx_ring, struct rte_ring *rep_ring_recv,
         int cpf_port, struct CPFConfig *config, 
         vector<map<unsigned long int, double>> *pcts,
         vector<float> *cpu_loads, vector<int> replica_ids, int propagation_delay) {
  this->id = id;
  this->rx_ring = rx_ring;
  this->rep_ring_recv = rep_ring_recv;
  this->config = config;
  this->pcts = pcts;
  this->cpu_loads = cpu_loads;
  this->replica_ids = replica_ids;
  this->propagation_delay;

}

CPF::~CPF() {}

void CPF::createDataWorker(){
  worker = new DataWorker(id, rx_ring, rep_ring, rep_ring_recv, state_map, config, 
                          this->pcts, this->cpu_loads, this->replica_ids,
                          this->propagation_delay);
}

void CPF::setRepRing(struct rte_ring *rep_ring) { 
  this->rep_ring = rep_ring; 
}

void CPF::setRepMempool(struct rte_mempool *rep_mempool) {
  this->rep_mempool = rep_mempool;

}

/* Main Routine of an CPF */
void CPF::start() {
  worker->setRepMempool(this->rep_mempool);
  worker->run();
  printf("Dropped count: %u\n", this->droped_count);
}

void CPF::startRecv() {
  worker->runRepRecv();
  // printf("Dropped count: %u\n", this->droped_count);
}


/* 
  This function is responsible for reception of replication messages from the 'runDataWorker' 
  thread and forwarding them to other CPFs being run on a separate server.
*/

void CPF::syncReplicas() {
  unsigned int res;
  unsigned int count = 0;
  struct rte_mbuf *rsp[SYNC_BATCH_SIZE];

  while (!force_quit) {

    // Replication messages reception.
    res = rte_ring_dequeue_burst(this->rep_ring, (void **)rsp,
                                SYNC_BATCH_SIZE, NULL);

    if (res == 0) {
      continue;
    }

    // Send message to CPFs deployed on a separate server.
    uint16_t to_send, sent;
    to_send = res;
    while (to_send != 0) {
      sent = rte_eth_tx_burst((uint8_t)REPLICATION_PORT, id, rsp + (res - to_send), to_send);
      to_send -= sent;
    }
    count += res;
  }

  printf("Total replication messages sent: %d\n", count);
}