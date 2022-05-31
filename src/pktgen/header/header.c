// This file describes the implementation of the custom header.

#include <sys/time.h>
#include <rte_memcpy.h>
#include "header.h"

const size_t C_HEADER_SIZE = sizeof(custom_header);

double timestamp_micro(void) {
    struct timeval now;
    gettimeofday(&now, 0);
    double timestamp;

    timestamp = now.tv_sec * 1e6;
    timestamp = timestamp + now.tv_usec;
    return timestamp;
}

void add_header(uint8_t * buff, guti_t guti, size_t payload_len) {
    custom_header header;

    header.guti = guti;
    header.payload_length = payload_len;
    header.timestamp = timestamp_micro();
    header.cpf_id = 0;
    header.last_msg = 0;
    rte_memcpy(buff, &header, sizeof(custom_header));
}

void update_header(uint8_t* src, uint8_t* dst, size_t payload_len) {
    custom_header * header = (custom_header*) dst;
    
    rte_memcpy(header, src, sizeof(custom_header));
    header->payload_length = payload_len;
}