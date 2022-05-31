#ifndef __state_h__
#define __state_h__

#include <vector>
#include <map>
#include "../../Common/DataType.h"

#include "server_states.h"

using namespace std;

/* State of a Connection is defined by the following class */
class State
{
protected:
	guti_t hash;
	guti_t guti;
	int last_msg_count;

	std::vector<int> HandleAttach(void *buff, size_t len);
	std::vector<int> HandleHandover(void *buff, size_t len);
	std::vector<int> HandleFastHandover(void *buff, size_t len);
	virtual size_t PrepareAttachMsg(int state, uint8_t msg[]);
	virtual size_t PrepareHandoverMsg(int state, uint8_t msg[]);

public:
	State();
	State(guti_t guti, guti_t hash, uint8_t payload_length);
	~State();
	guti_t getGuti();
	void setGuti(guti_t guti);
	guti_t getHash();
	void setHash(guti_t hash);
    int getProcedureType();

	virtual std::vector<int> UpdateState(void *buff, size_t len);
	virtual size_t PrepareMsg(int state, uint8_t buff[]);

	StateMachine state_machine;
	double startTime;
	double endTime;

	bool completed;
    
	unsigned long int completed_logicalClock;
};

class Asn1State : public State
{
private:
	guti_t hash;
	guti_t guti;
	std::vector<int> HandleAttach(void *buff, size_t len);
	std::vector<int> HandleHandover(void *buff, size_t len);
	std::vector<int> HandleFastHandover(void *buff, size_t len);
    std::vector<int> HandleServiceRequest(void *buff, size_t len);
    std::vector<int> HandleDPCMServiceRequest(void *state, size_t len);
	size_t PrepareAttachMsg(int state, uint8_t msg[]);
	size_t PrepareHandoverMsg(int state, uint8_t msg[]);
    size_t PrepareServiceRequestMsg(int state, uint8_t msg[]);

public:
    Asn1State::Asn1State();
	Asn1State(guti_t guti, int procedure_type);
	Asn1State(guti_t guti, guti_t hash, size_t payload_length);
	std::vector<int> UpdateState(void *buff, size_t len);
	size_t PrepareMsg(int state, uint8_t buff[]);
};

class FlatbuffersState : public State
{
private:
	guti_t hash;
	guti_t guti;
	std::vector<int> HandleAttach(void *buff, size_t len);
	std::vector<int> HandleHandover(void *buff, size_t len);
	std::vector<int> HandleServiceRequest(void *buff, size_t len);
	std::vector<int> HandleFastHandover(void *buff, size_t len);
	size_t PrepareAttachMsg(int state, uint8_t msg[]);
	size_t PrepareHandoverMsg(int state, uint8_t msg[]);
	size_t PrepareServiceRequestMsg(int state, uint8_t msg[]);

public:
	FlatbuffersState(guti_t guti, guti_t hash, size_t payload_length);
	std::vector<int> UpdateState(void *buff, size_t len);
	size_t PrepareMsg(int state, uint8_t buff[]);
};

#endif
