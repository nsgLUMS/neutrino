#ifndef __UDP_h__
#define __UDP_h__

#include <netinet/in.h>

class Request
{
private:
	uint8_t *msg;
	size_t size = 0;
	double arrivalTime = 0;

public:
	Request(uint8_t msg[], double time, size_t size);
	~Request();
	uint8_t *getMsg();
	size_t getSize();
	void setSize(size_t size);
	double getArrivalTime();
};

#endif
