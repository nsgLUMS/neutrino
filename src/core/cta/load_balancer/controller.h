#ifndef __CONTROLLER__
#define __CONTROLLER__

#include "lbaas/load_balancer.h"

class Controller
{
  public:
	LoadBalancer *lb;

	Controller();
	~Controller();

	void add_l1_cpf(CPF * cpf);
	void add_l2_cpf(CPF * cpf);
};

#endif
