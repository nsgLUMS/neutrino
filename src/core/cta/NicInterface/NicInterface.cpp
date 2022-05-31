#include <rte_mbuf.h>

#include "NicInterface.h"
#include "../mem_store/store_wrapper.h"

static bytesToNumber converter;
static logicalClockExtractor ext;

int NicInterface::_dequeue(response &r, rte_hash *logical_clocks, LBInfo *conf)
{
	void *tmp;
	int res = rte_ring_dequeue(this->rx_rings[conf->id], &tmp);

	if (res != 0)
		return;

	uint8_t *data = (uint8_t *)tmp;

	ext.buff[0] = data[10];
	ext.buff[1] = data[11];
	ext.buff[2] = data[12];
	ext.buff[3] = data[13];
	ext.buff[4] = data[14];
	ext.buff[5] = data[15];
	ext.buff[6] = data[16];
	ext.buff[7] = data[17];

	if (rte_hash_lookup(logical_clocks, &ext.logical_clock) != -ENOENT){

		int count;
		rte_hash_lookup_data(logical_clocks, &ext.logical_clock, (void **) &count);
		rte_hash_add_key_data(logical_clocks, &ext.logical_clock, (void *) ++count);

		if (count == conf->config->replicas){
			rte_hash_del_key(logical_clocks, &ext.logical_clock);
		}

		rte_mempool_put(global_mempool, data + 2);
		return 0;

	}
	else
	{
		int *num = 1;
		rte_hash_add_key_data(logical_clocks, &ext.logical_clock, (void *) num);

		if (conf->config->replicas == 1)
		{
			rte_hash_del_key(logical_clocks, &ext.logical_clock);
		}

		converter.buff[0] = data[0];
		converter.buff[1] = data[1];

		r.data = data + 2;
		r.size = converter.value;

		return r.size;
	}
}

/* 
   The function is called by 'processPkt' thread when CTA receives a message 
   from the traffic generator.
   It forwards the message to the selected CPF.
*/
void NicInterface::OnPayloadReceived(struct rte_mbuf *rsp, CustomAppHeader header, 
									 int queue_id, int destination_port)
{
	string guti_str = std::to_string(header.GUTI);
	if (pkt_logging)
	{
		uint8_t *data = rte_pktmbuf_mtod_offset(rsp, uint8_t *, PAYLOAD_OFFSET);
		save(header.GUTI, header.logicalClock, data, header.payloadLength);

		if (header.last_msg){
			done(header.GUTI, header.logicalClock);
		}
	}



	controller->lb->forward(rsp, controller->lb->hash(guti_str), queue_id,
							destination_port);
}

// Retreive data to be transmitted to eNB/Traffic Generator.
uint8_t NicInterface::TryGetData(rte_hash *logical_clocks, response r[MAX_PKT_BURST],
								 uint8_t burst, LBInfo *conf)
{

	uint8_t b = RTE_MIN(burst, rte_ring_count(rx_rings[conf->id]));
	if (b == 0)
		return 0;

	uint8_t i = 0, j = 0;
	while (j < b)
	{
		if (_dequeue(r[i], logical_clocks, conf) > 0)
		{
			i++;
		}
		j++;
	}

	return i;
}

// All initializations will go here.
void NicInterface::Init()
{
	srand(time(NULL));
}

void NicInterface::setRxRings(struct rte_ring **rx_rings)
{
	this->rx_rings = rx_rings;
}

NicInterface::NicInterface()
{
	controller = new Controller();
}

NicInterface::~NicInterface() { }
