#pragma once
#include <memory>

#include "../Common/DataType.h"
#include "../Common/globals.h"
#include "../Common/custom_header/CustomHeaderParser.h"
#include <rte_ring.h>
#include <rte_hash.h>

#include "../load_balancer/controller.h"

struct response {
	uint8_t *data;
	size_t size;
};

typedef struct response response;

class NicInterface
{
private:
	struct rte_ring **rx_rings;
	int _dequeue(response &r, rte_hash *logical_clocks, LBInfo *conf);

public:
	// Do all one time initializations here.
	void Init();

	// Called when data packet is received from the eNB/Traffic Generator.
	void OnPayloadReceived(struct rte_mbuf *rsp, CustomAppHeader header, 
						   int queue_id, int destination_port);
	uint8_t TryGetData(rte_hash *logical_clocks, response r[MAX_PKT_BURST], uint8_t burst, 
					   LBInfo *conf);
	void setRxRings(struct rte_ring **rx_rings);

	Controller *controller = NULL;

	NicInterface();
	~NicInterface();

};
