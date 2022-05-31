#include "controller.h"

Controller::Controller()
{
	this->lb = new LoadBalancer();
}

Controller::~Controller() {}

void Controller::add_l1_cpf(CPF * cpf) {
	this->lb->addL1Node(cpf);
}

void Controller::add_l2_cpf(CPF * cpf) {
	this->lb->addL2Node(cpf);
}