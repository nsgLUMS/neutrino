#ifndef __lb_h__
#define __lb_h__

#include "../map/map.h"
#include "../../cpf/cpf.h"
#include <set>

class LoadBalancer
{
private:
	CircularMap<size_t, CPF *> l1_ring;
	CircularMap<size_t, CPF *> l2_ring;

public:
	LoadBalancer();
	~LoadBalancer();
	void forward(struct rte_mbuf *rsp, size_t key, 
				 int ring_index, int destination_port);
	CPF* nextSuitableNode(size_t key, int destination_port);
	void addL1Node(CPF *new_cpf);
	void addL2Node(CPF *new_cpf);
	size_t hash(string key);
	void showStatus();
};

#endif
