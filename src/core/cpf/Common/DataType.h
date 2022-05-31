#pragma once
#include <string>

typedef unsigned long int guti_t;
typedef unsigned long int lgclock_t;
// This custom App header will be on top of S1AP protocol.
typedef struct CustomAppHeader
{
	double timeStamp;
	lgclock_t logicalClock;
	size_t payloadLength;
	// GUTI value for each UE
	guti_t GUTI;
	uint8_t cpf_id;
	uint8_t last_msg;

} CustomAppHeader;

typedef struct ControlMessage
{
	CustomAppHeader header;
	uint8_t actualMessage[1024];
} ControlMessage;
