#include "load_balancer.h"
#include <iostream>
#include <rte_ethdev.h>
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_udp.h>

#include "../../Common/DataType.h"
#include "../../init/initializer.hpp"

const size_t CPF_ID_OFFSET =
    sizeof(double) + sizeof(lgclock_t) + sizeof(size_t) + sizeof(guti_t); 

using namespace std;

LoadBalancer::LoadBalancer() {}

LoadBalancer::~LoadBalancer() {}

void LoadBalancer::addL2Node(CPF *new_cpf)
{
  int key = this->hash(to_string(new_cpf->getCPFPort()));
  new_cpf->setHash(key);
  l2_ring.insert(key, new_cpf);

  for (int i = 1; i < 400; i++)
  {
    key = this->hash(to_string(new_cpf->getHash() + i * 1373));
    l2_ring.insert(key, new_cpf);
  }
}

void LoadBalancer::addL1Node(CPF *new_cpf)
{
  int key = this->hash(to_string(new_cpf->getCPFPort()));
  new_cpf->setHash(key);
  l1_ring.insert(key, new_cpf);

  for (int i = 1; i < 400; i++)
  {
    key = this->hash(to_string(new_cpf->getHash() + i * 1373));
    l1_ring.insert(key, new_cpf);
  }
}

size_t LoadBalancer::hash(string key)
{
  std::hash<string> str_hash;
  return str_hash(key) % 999999937;
}

CPF* LoadBalancer::nextSuitableNode(size_t key, int destination_port)
{
  if (destination_port == LOCAL_CPF_PORT)
  {
    return l1_ring.upper(key);
  }
  else {
    return l2_ring.upper(key);
  }
}

/*
  This function is responsible for searching suitable 
  CPFs in a consistent hash ring for each message. After it, each message is 
  sent to the CPF server.
*/
void LoadBalancer::forward(struct rte_mbuf *rsp, size_t key, 
                           int queue_id, int destination_port)
{

  if (l1_ring.size() > 0)
  {

    // Search the consistent hash to find a CPF for a message.
    CPF *cpf = this->nextSuitableNode(key, destination_port);

    uint8_t *payload = rte_pktmbuf_mtod_offset(rsp, uint8_t *, PAYLOAD_OFFSET);

    // Embed CPF ID in the message payload
    *(payload + CPF_ID_OFFSET) = cpf->getID();

    int port;
    
    // Update the destination port.
    if (cpf->getType() == 0){
      port = LOCAL_CPF_PORT;
      update_tx(LOCAL_CPF_PORT, 1);
    } else {
      port = REMOTE_CPF_PORT;
      update_tx(REMOTE_CPF_PORT, 1);
    }

    // Send the message to a CPF through the selected port.
    rte_eth_tx_burst(port, queue_id, &rsp, 1);

  }
}

// Shows size of L1 and L2 ring.
void LoadBalancer::showStatus()
{
  cout << "Nodes on L1 Ring = " << l1_ring.size() << endl
       << std::flush;
  cout << "+++++=====+++++=====+++++\n"
       << std::flush;
  cout << "Nodes on L2 Ring = " << l2_ring.size() << endl
       << std::flush;
  cout << "+++++=====+++++=====+++++\n"
       << std::flush;

}
