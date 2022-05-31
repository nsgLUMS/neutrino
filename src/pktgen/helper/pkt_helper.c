#include "pkt_helper.h"
#include "globals.h"
#include <rte_ether.h>

struct ether_addr zero_mac = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
struct ether_addr ether_dst = {{0x68, 0x05, 0xca, 0x00, 0x00, 0x01}};

void display_pkt(uint8_t *buf, size_t len) {
  size_t i;
  for (i = 0; i < len; i++) {
    printf("%u, ", buf[i]);
  }
  printf("\n");
}

// Construct a packet on top of the given message. Including ethernet, ip , and udp header. 
uint16_t generate_packet(struct rte_mbuf *buf,
                                       uint16_t payload_size) {
  struct ether_hdr *eth_hdr;
  struct ipv4_hdr *ip_hdr;
  struct udp_hdr *udp_hdr;
  uint16_t pkt_size = HEADER_SIZE + payload_size;

  buf->l2_len = sizeof(struct ether_hdr);
  buf->l3_len = sizeof(struct ipv4_hdr);
  buf->l4_len = sizeof(struct udp_hdr);
  buf->ol_flags = PKT_TX_IPV4 | PKT_TX_IP_CKSUM | PKT_TX_UDP_CKSUM;

  buf->pkt_len = pkt_size;
  buf->data_len = pkt_size;

  eth_hdr = rte_pktmbuf_mtod(buf, struct ether_hdr *);
  // Set destination address in ethernet headers.
  if (is_zero_ether_addr(&zero_mac)) {
    ether_addr_copy(&ether_dst, &eth_hdr->d_addr);
  } else {
    ether_addr_copy(&zero_mac, &eth_hdr->d_addr);
  }

  // Set source address in ethernet headers.
  if (is_zero_ether_addr(&zero_mac)) {
    ether_addr_copy(&zero_mac, &eth_hdr->s_addr);
  } else {
    ether_addr_copy(&zero_mac, &eth_hdr->s_addr);
  }

  eth_hdr->ether_type = rte_cpu_to_be_16(ETHER_TYPE_IPv4);

  // Construct ip header.
  ip_hdr = (struct ipv4_hdr *)(eth_hdr + 1);
  ip_hdr->type_of_service = 0;
  ip_hdr->fragment_offset = 0;
  ip_hdr->time_to_live = 64;
  ip_hdr->next_proto_id = IPPROTO_UDP;
  ip_hdr->packet_id = 0;
  ip_hdr->version_ihl = (1 << 6) + 5;
  ip_hdr->total_length = rte_cpu_to_be_16(pkt_size - sizeof(struct ether_hdr));
  ip_hdr->src_addr = 0xAABB;
  ip_hdr->dst_addr = 0xCCDD;
  ip_hdr->hdr_checksum = 0;

  // Construct udp header.
  udp_hdr = (struct udp_hdr *)(ip_hdr + 1);
  udp_hdr->src_port = 0xAABB;
  udp_hdr->dst_port = 0xCCDD;
  udp_hdr->dgram_cksum = 0;
  udp_hdr->dgram_len = rte_cpu_to_be_16(pkt_size - sizeof(struct ether_hdr) -
                                        sizeof(struct ipv4_hdr));
  return pkt_size;
}