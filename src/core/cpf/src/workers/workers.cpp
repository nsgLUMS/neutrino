#include <rte_cycles.h>
#include <rte_ip.h>
#include <rte_ethdev.h>
#include <rte_ring.h>
#include <rte_udp.h>
#include <unistd.h>

#include <mutex>

#include "workers.h"

#include "../../Common/globals.h"
// #include "../../Common/messages/messages.hpp"
#include "../../Common/time/time.h"

int maxProcedures = 20000000;
std::mutex print_mtx;

const size_t PAYLOAD_OFFSET =
    sizeof(struct ether_hdr) + sizeof(struct ipv4_hdr) + sizeof(struct udp_hdr);

const size_t FLAG_OFFSET =
    sizeof(double) + sizeof(lgclock_t) + sizeof(size_t) + sizeof(guti_t) + sizeof(uint8_t);

static struct ether_addr ether_dst = {{0x68, 0x05, 0xca, 0x00, 0x00, 0x01}};
static struct ether_addr zero_mac = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

DataWorker::DataWorker(uint16_t id, struct rte_ring *rx_ring, struct rte_ring *rep_ring,
                       struct rte_ring *rep_ring_recv,
                       CircularMap<unsigned long int, State *> &state_map,
                       struct CPFConfig *config,
                       vector<map<unsigned long int, double>> *pcts,
                       vector<float> *cpu_loads, vector<int> replica_ids, int propagation_delay)
    : state_map(state_map)
{
  this->rx_ring = rx_ring;
  this->rep_ring = rep_ring;
  this->rep_ring_recv = rep_ring_recv;
  this->config = config;
  this->pcts = pcts;
  this->cpu_loads = cpu_loads;
  this->id = id;
  this->replica_ids = replica_ids;
  this->propagation_delay;
}

DataWorker::~DataWorker() {}

void DataWorker::setRepMempool(struct rte_mempool *rep_mempool)
{
  this->rep_mempool = rep_mempool;
}

static inline uint16_t generate_packet(struct rte_mbuf *buf, uint8_t *response,
                                       uint32_t size)
{
  buf->ol_flags = PKT_TX_IPV4 | PKT_TX_IP_CKSUM | PKT_TX_UDP_CKSUM;
  buf->l2_len = sizeof(struct ether_hdr);
  buf->l3_len = sizeof(struct ipv4_hdr);
  buf->l4_len = sizeof(struct udp_hdr);

  struct ether_hdr *eth_hdr;
  struct ipv4_hdr *ip_hdr;
  struct udp_hdr *udp_hdr;
  struct tcp_hdr *tcp_hdr;
  uint16_t pkt_size = MAX_PKT_SIZE;

  buf->pkt_len = pkt_size;
  buf->data_len = pkt_size;

  eth_hdr = rte_pktmbuf_mtod(buf, struct ether_hdr *);
  if (is_zero_ether_addr(&zero_mac))
  {
    ether_addr_copy(&ether_dst, &eth_hdr->d_addr);
  }
  else
  {
    ether_addr_copy(&zero_mac, &eth_hdr->d_addr);
  }

  if (is_zero_ether_addr(&zero_mac))
  {
    ether_addr_copy(&zero_mac, &eth_hdr->s_addr);
  }
  else
  {
    ether_addr_copy(&zero_mac, &eth_hdr->s_addr);
  }
  eth_hdr->ether_type = rte_cpu_to_be_16(ETHER_TYPE_IPv4);

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

  udp_hdr = (struct udp_hdr *)(ip_hdr + 1);
  udp_hdr->src_port = 0xAABB;
  udp_hdr->dst_port = 0xCCDD;
  udp_hdr->dgram_cksum = 0;
  udp_hdr->dgram_len =
      rte_cpu_to_be_16(pkt_size - sizeof(struct ether_hdr) - sizeof(*ip_hdr));
      
  if (response != nullptr){
    uint8_t *ptr = rte_pktmbuf_mtod_offset(buf, uint8_t *, PAYLOAD_OFFSET);
    rte_memcpy((void *)ptr, response, size);
  }

  return pkt_size;
}

inline uint16_t DataWorker::enqueue_bulk(State **states, uint16_t n)
{
  if (n == 0)
  {
    return 0;
  }

  int ret;

  // Initializing new replication messages.
  uint8_t *cpy[n];


  ret = rte_mempool_get_bulk(rep_mempool, (void **)cpy, n);
  if (ret != 0)
  {
    printf("Cannot get memory from rep mempool from CPF id = %u\n", id);
    return 0;
  }

  struct rte_mbuf *rsp[n];

  for (uint16_t i = 0; i < n; i++)
  {

    rsp[i] = rte_pktmbuf_alloc(global_mempool);
    if (rsp[i] == NULL){
      printf("Failed to allocate from mempool for replication\n");
      continue;
    }

    CustomAppHeader header;

    // Filling a new application header fileds.
    header.GUTI = states[i]->getGuti();
    header.payloadLength = sizeof(CustomAppHeader) + sizeof(Asn1State);
    header.cpf_id = replica_ids[i];

    // Append the application header to the new replication message.
    rte_memcpy((void *)(cpy[i]), (void *)&header, sizeof(CustomAppHeader));

    // Copy the user state to the new replication message.
    rte_memcpy((void *)(cpy[i]) + sizeof(CustomAppHeader), 
               (void *)states[i], sizeof(Asn1State));

    // Attach protocol headers to the message.
    generate_packet(rsp[i], cpy[i], sizeof(CustomAppHeader) + sizeof(Asn1State));
  }

  unsigned int r = rte_ring_enqueue_burst(this->rep_ring, (void **)rsp, n, NULL);
  rte_mempool_put_bulk(rep_mempool, (void **)cpy, n);
  return 1;
}

inline uint16_t DataWorker::replicate_state(State *s)
{
  State *cpy[replica_ids.size()];

  // Create message copies to send them to each replica of a particular CPF.
  for (int i = 0; i < replica_ids.size(); i++){
    cpy[i] = s;
  }

  return enqueue_bulk(cpy, replica_ids.size());
}

/* 
  This thread is responsible for copying/updating the user state coming from a CPF.
 */
void DataWorker::runRepRecv(){

  struct rte_mbuf *pkt_burst[SYNC_BATCH_SIZE];
  int nb_rx;

  while (!force_quit){
    nb_rx = rte_ring_dequeue_burst(this->rep_ring_recv, (void **)pkt_burst,
                                 SYNC_BATCH_SIZE, NULL);

    if (nb_rx == 0) {
      continue;
    }

    for (int pkt = 0; pkt < nb_rx; pkt++) {
      uint8_t *data = rte_pktmbuf_mtod_offset(pkt_burst[pkt], uint8_t*, PAYLOAD_OFFSET);

      // Parsing the receivned messages to access the individual fields.
      headerParser.Parse(data);
      double time = TimeStampMicro();

      Request *request = new Request(
          data, time, headerParser.controlMessage.header.payloadLength);

      guti_t hash = headerParser.controlMessage.header.GUTI;

      // Create and copy the received state to the new state variable.
      State *s = new Asn1State();
      memcpy(s, data + sizeof(CustomAppHeader), sizeof(Asn1State));

      State *new_s = new Asn1State(s->getGuti(), s->getProcedureType());
      new_s->startTime = request->getArrivalTime();

      // Insert the new/updated state into the state map.
      state_map.insert(hash, new_s);

    }

    for (int k = 0; k < nb_rx; k++){
      rte_pktmbuf_free(pkt_burst[k]);
    }


  }
}

/* 
  These workers acts as a CPFs. A single CPF's responsibilites are given
  below,

  1. Receiving messages from the do_rx worker defined in 'main.c'
  2. Processing and generating response for the messages.
  3. Sending the responses back to CTA. 
 */

void DataWorker::run()
{
  uint32_t rep_pkts = 0;
  uint32_t pkt_count = 0;
  uint32_t response_count = 0;

  double start_time = 0, end_time, complete_end_time;

  vector<pair<unsigned long int, double>> attach_pcts;
  attach_pcts.reserve(maxProcedures);
  vector<pair<unsigned long int, double>> handover_pcts;
  handover_pcts.reserve(maxProcedures);
  vector<pair<unsigned long int, double>> service_pcts;
  service_pcts.reserve(maxProcedures);

  double max_guti = 0;
  int new_msgs = 0;
  int res_msgs = 0;

  const uint64_t hz = rte_get_timer_hz();
  double processing_cycles = 0, total_cycles = -1;

  double current_cycles = rte_get_timer_cycles();
  uint16_t nb_rx;
  struct rte_mbuf *pkt_burst[MAX_PKT_BURST];
  struct rte_mbuf *rsp[MAX_PKT_BURST];

  int len;
  struct rte_eth_xstat *xstats;
  struct rte_eth_xstat_name *xstats_names;

  while (!force_quit)
  {

    // Receiving messages from the 'do_rx' thread. 
    nb_rx =
        rte_ring_dequeue_burst(rx_ring, (void **)pkt_burst, MAX_PKT_BURST, NULL);
    if (nb_rx == 0)
      continue;

    for (int pkt = 0; pkt < nb_rx; pkt++) {
      uint8_t *data = rte_pktmbuf_mtod_offset(pkt_burst[pkt], uint8_t*, PAYLOAD_OFFSET);

      rsp[pkt] = rte_pktmbuf_alloc(global_mempool);
      if (rsp[pkt] == NULL){
        printf("Failed to allocate from mempool\n");
        continue;
      }

      if (total_cycles != -1)
      {
        total_cycles += (double)(rte_get_timer_cycles() - current_cycles) / hz;
        current_cycles = rte_get_timer_cycles();
      }

      if (unlikely(pkt_count == 0))
      {
        start_time = TimeStampMicro();
        total_cycles = (double)(rte_get_timer_cycles() - current_cycles) / hz;
        current_cycles = rte_get_timer_cycles();
      }

      
      pkt_count++;

      uint8_t *req = (uint8_t *)data;
      double time = TimeStampMicro();

      // Parse message header to get access to the individual fields.
      headerParser.Parse(req);

      Request *request = new Request(
          req, time, headerParser.controlMessage.header.payloadLength);

      guti_t hash = headerParser.controlMessage.header.GUTI;

      // Per-user state at each CPF.
      State *state;
      if (state_map.count(hash))
      {
        state = state_map.get(hash);
        if (headerParser.controlMessage.header.payloadLength == 423){
          state->startTime = request->getArrivalTime();
        }
      }
      else
      {
        if (config->serializer == 0)
        {
          state = new Asn1State(headerParser.controlMessage.header.GUTI, hash,
                                headerParser.controlMessage.header.payloadLength);
          state->startTime = request->getArrivalTime();
          state_map.insert(hash, state);
        }
        else if (config->serializer == 1)
        {
          state = new FlatbuffersState(
              headerParser.controlMessage.header.GUTI, hash,
              headerParser.controlMessage.header.payloadLength);
          state->startTime = request->getArrivalTime();
          state_map.insert(hash, state);
        }
        else
        {
          cout << "Wrong serializer mentioned in config file..." << endl;
        }
        new_msgs++;
        state->setHash(hash);
        state->setGuti(headerParser.controlMessage.header.GUTI);
      }

      // Process the incoming procedure message and identify the response message(s)
      // by executing the relevant procedure state machine. 
      std::vector<int> next_msgs =
          state->UpdateState((void *)headerParser.controlMessage.actualMessage,
                            headerParser.controlMessage.header.payloadLength);


      if (next_msgs.size() > 0)
      {
        for (auto it = next_msgs.begin(); it != next_msgs.end(); ++it)
        {
          // If is the last message of procedure, record its completion time
          // in an array.
          if (*it == 99)
          {
            state->completed = true;

            state->completed_logicalClock =
                headerParser.controlMessage.header.logicalClock;
            state->endTime = request->getArrivalTime();

            pair<unsigned long int, double> pair;
            pair.first = state->getGuti();
            pair.second = state->endTime - state->startTime;

            if (state->getProcedureType() == 0 || state->getProcedureType() == 2){
              attach_pcts.push_back(pair);
            } else if (state->getProcedureType() == 6){
              handover_pcts.push_back(pair);
            } else if (state->getProcedureType() == 4){
              service_pcts.push_back(pair);
            }

            // If per procedure replication is enabled
            if (config->replication == 2)
            {
              rep_pkts += replicate_state(state);
            }

            complete_end_time = TimeStampMicro();
            break;
          }
          uint8_t buff[1024] = {0};

          // Generate the identified procedure message(s).
          size_t bytes_to_send = state_map.get(hash)->PrepareMsg(*it, buff);
          
          if (bytes_to_send == 0)
          {
            break;
          }

          uint8_t *payload_ptr = rte_pktmbuf_mtod_offset(rsp[response_count], 
                                                         uint8_t *, PAYLOAD_OFFSET);

          // Add application header to the message. 
          bytes_to_send =
              headerParser.PrependCustomHeader(buff, payload_ptr, bytes_to_send);

          // Attach protocol headers to the message
          generate_packet(rsp[response_count], NULL, bytes_to_send);
          response_count++;
        }
      }
      delete request;
      end_time = TimeStampMicro();
      processing_cycles += (double)(rte_get_timer_cycles() - current_cycles) / hz;
    }

    // Return back the responses to the CTA.
    uint16_t to_send = response_count;
    int sent;
    while (to_send != 0) {
      sent = rte_eth_tx_burst((uint8_t) PORT, this->id, rsp + (response_count - to_send), to_send);
      res_msgs += sent;
      to_send -= sent;
    }

    for (int k = 0; k < nb_rx; k++){
      rte_pktmbuf_free(pkt_burst[k]);
    }
    response_count = 0;
  }

  len = rte_eth_xstats_get(PORT, NULL, 0);
  if (len < 0)
      rte_exit(EXIT_FAILURE,
              "rte_eth_xstats_get(%u) failed: %d", PORT,
              len);

  xstats = calloc(len, sizeof(*xstats));
  if (xstats == NULL)
      rte_exit(EXIT_FAILURE,
              "Failed to calloc memory for xstats");


  int ret = rte_eth_xstats_get(PORT, xstats, len);
  if (ret < 0 || ret > len) {
      free(xstats);
      rte_exit(EXIT_FAILURE,
              "rte_eth_xstats_get(%u) len%i failed: %d",
              PORT, len, ret);
  }

  xstats_names = calloc(len, sizeof(*xstats_names));
  if (xstats_names == NULL) {
      free(xstats);
      rte_exit(EXIT_FAILURE,
              "Failed to calloc memory for xstats_names");
  }

  ret = rte_eth_xstats_get_names(PORT, xstats_names, len);
  if (ret < 0 || ret > len) {
      free(xstats);
      free(xstats_names);
      rte_exit(EXIT_FAILURE,
              "rte_eth_xstats_get_names(%u) len%i failed: %d",
              PORT, len, ret);
  }

  printf("Len: %d\n", len);
  for (int i = 0; i < len; i++) {
      if (xstats[i].value > 0)
          printf("Port %u: %s:\t\t%"PRIu64"\n",
                  PORT,
                  xstats_names[i].name,
                  xstats[i].value);
  }

  double elapsed_complete = (complete_end_time - start_time) * 1e-6;
  double elapsed = (end_time - start_time) * 1e-6;
  double proc_rate = pkt_count / elapsed;
  printf("New messages: %d | Res messages: %d\n", new_msgs, res_msgs);
  printf("Response count %d\n", response_count);
  print_mtx.lock();


  // TODO: Improve procedures pcts recording implementation.
  for (auto it = attach_pcts.begin(); it != attach_pcts.end(); ++it)
  {

    if ((*this->pcts)[0].find((*it).first) == (*this->pcts)[0].end())
    {
        (*this->pcts)[0][(*it).first] = (*it).second;
    }
    else if ((*it).second < (*this->pcts)[0][(*it).first])
    {
        (*this->pcts)[0][(*it).first] = (*it).second;
    }
  }

  for (auto it = handover_pcts.begin(); it != handover_pcts.end(); ++it)
  {
    if ((*this->pcts)[1].find((*it).first) == (*this->pcts)[1].end())
    {
      if (this->id != 0 && this->id != 1){
        (*this->pcts)[1][(*it).first] = (*it).second + (this->propagation_delay * 8);
      } else {
        (*this->pcts)[1][(*it).first] = (*it).second;
      }
    }
    else if ((*it).second < (*this->pcts)[1][(*it).first])
    {
      if (this->id != 0 && this->id != 1){
        (*this->pcts)[1][(*it).first] = (*it).second + (this->propagation_delay * 8);
      } else {
        (*this->pcts)[1][(*it).first] = (*it).second;
      }
    }
  }

  for (auto it = service_pcts.begin(); it != service_pcts.end(); ++it)
  {
    if ((*this->pcts)[2].find((*it).first) == (*this->pcts)[2].end())
    {
        (*this->pcts)[2][(*it).first] = (*it).second;
    }
    else if ((*it).second < (*this->pcts)[2][(*it).first])
    {
        (*this->pcts)[2][(*it).first] = (*it).second;
    }
  }


  (this->cpu_loads)->push_back((processing_cycles / total_cycles) * 100);
  printf("Processed %u packets\n", pkt_count);
  print_mtx.unlock();
  return;
}

