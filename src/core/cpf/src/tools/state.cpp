#include "state.h"
#include "../map/map.h"
#include "time.h"

#include "../../Common/DataType.h"

State::State()
{
	state_machine = StateMachine();
	startTime = 0;
}

State::State(guti_t guti, unsigned long int hash, uint8_t payload_length) : guti(guti), hash(hash)
{
	state_machine = StateMachine();
}

State::~State() {}

guti_t State::getGuti()
{
	return guti;
}

void State::setGuti(guti_t guti)
{
	this->guti = guti;
}

void State::setHash(unsigned long int hash)
{
	this->hash = hash;
}

unsigned long int State::getHash()
{
	return hash;
}

int State::getProcedureType(){
	this->state_machine.getProcedureType();
}

size_t State::PrepareAttachMsg(int state, uint8_t msg[]) {}

std::vector<int> State::HandleAttach(void *buff, size_t len) {}

size_t State::PrepareHandoverMsg(int state, uint8_t msg[]) {}

size_t State::PrepareMsg(int state, uint8_t msg[]) {}

std::vector<int> State::UpdateState(void *buff, size_t len) {}
