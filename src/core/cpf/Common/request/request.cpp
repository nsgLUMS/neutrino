#include "request.h"

Request::Request(uint8_t msg[], double time, size_t size)
{
	this->size = size;
	this->msg = msg;
	this->arrivalTime = time;
}

Request::~Request() { }

double Request::getArrivalTime()
{
	return arrivalTime;
}

size_t Request::getSize()
{
	return this->size;
}

void Request::setSize(size_t size)
{
	this->size = size;
}

uint8_t *Request::getMsg()
{
	return this->msg;
}
