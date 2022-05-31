// This is wrapper around the store and store's request.
// These functions should be called directly from the program.

#include <rte_ring.h>

#include "store_wrapper.h"

#include "store.h"
#include "requests.h"
#include "../Common/globals.h"

static rte_ring *ring = NULL;

void send(StoreRequest *r)
{
	if (unlikely(rte_ring_enqueue(ring, (void *)r) != 0)) {
		printf("Cannot enqueue in the ring buffer of store\n");
		printf("Ring buffer's state: total entries %u free entries %u\n", rte_ring_count(ring), rte_ring_free_count(ring));
	}
}

// Spawns a thread for memory logger
void init_store(void *args)
{
	ring = (struct rte_ring *)args;
	init(ring);
}

void save(hash_t hash, lgclock_t clock, uint8_t const *msg, size_t msg_len)
{
	void *local_msg = NULL;
	// TODO: Make sure to put this memeory back in mempool.
	rte_mempool_get(global_mempool, &local_msg);
	if (local_msg == NULL) {
		printf("Cannot get memory for store request.\n");
		return;
	}
	rte_memcpy(local_msg, (void *)msg, msg_len);

	StoreRequest *r = set_request(hash, clock, local_msg, msg_len);
	if (r == NULL) {
		rte_mempool_put(global_mempool, local_msg);
	} else {
		send(r);
	}
}

void done(hash_t hash, lgclock_t clock)
{
	StoreRequest *r = done_request(hash, clock);
	// IMPORTANT NOTE:
	// If the following condition is false anytime of the execution it means
	// there was a request to free an chunk from the store but we were unable
	// to get the space from mempool to send a request to the store to free 
	// the memory. So the required memory will never be free.
	// I think we should redesign our memory store -- Azam
	if (r != NULL) {
		send(r);
	}
}

void synced(hash_t hash)
{
}

void replay(hash_t hash)
{
}
