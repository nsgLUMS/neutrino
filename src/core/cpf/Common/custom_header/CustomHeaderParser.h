#pragma once
#include "../DataType.h"

class CustomHeaderParser
{
public:
	uint8_t *controlMessagePointer;

	// Control messge length in bytes;
	size_t controlMessgaeLength;

public:
	ControlMessage controlMessage;
	int MAX_PAYLOAD_LENGTH = 1024;

public:
	int PrependCustomHeader(uint8_t *payload, uint8_t * newPayload, size_t length);
	void Parse(uint8_t *data);
	CustomHeaderParser();
	~CustomHeaderParser();
};
