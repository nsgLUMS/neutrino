#include <rte_mbuf.h>

#include "CustomHeaderParser.h"

void CustomHeaderParser::Parse(uint8_t *data) {
    controlMessagePointer = data;
    memcpy(&controlMessage.header, data, sizeof(controlMessage.header));
    size_t headerLength = sizeof(controlMessage.header);

    controlMessgaeLength = headerLength;
    controlMessgaeLength += controlMessage.header.payloadLength;
    memcpy(controlMessage.actualMessage, data + headerLength, controlMessage.header.payloadLength);
}

int CustomHeaderParser::PrependCustomHeader(uint8_t *payload, uint8_t *newPayload, size_t length)
{
    // Update length field in the custom header.
    // Do any other change in the custom header, if required.
    controlMessage.header.payloadLength = length;
    int customHeaderSize = sizeof(controlMessage.header);

    // Add updated custom application header to the message.
    memcpy((void *)(newPayload), &controlMessage.header, customHeaderSize);

    // Pointer to the payload, first byte after the custom application header.
    uint8_t *payloadPointer = newPayload + customHeaderSize;

    // Copy payload at the end of custom application header.
    memcpy((void *)(payloadPointer), payload, length);

    return customHeaderSize + length;
}

CustomHeaderParser::CustomHeaderParser() { }

CustomHeaderParser::~CustomHeaderParser() { }
