#ifndef PKT_HELPER_H_
#define PKT_HELPER_H_

#include <ctype.h>
#include <rte_mbuf.h>

uint16_t generate_packet(struct rte_mbuf *buf, uint16_t payload_size);

#endif