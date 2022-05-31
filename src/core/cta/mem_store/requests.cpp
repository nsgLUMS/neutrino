// The main request structure used to communicate with the memory logger
// It should expose certain messages depending upon the type
// of the functionality provided by the memory logger.

#include "requests.h"

#include "../Common/globals.h"

void rfree(StoreRequest *r) {
	rte_mempool_put(global_mempool, r);
}

StoreRequest * _ralloc() {
	void *ptr = NULL;
	rte_mempool_get(global_mempool, &ptr);
	if (ptr == NULL) {
		printf("Error! Cannot allocate memory for StoreRequest.\n");
		return NULL;
	}
	return (StoreRequest *) ptr;
}

StoreRequest * set_request(hash_t hash, lgclock_t clock, uint8_t const *msg, size_t msg_len) {
	StoreRequest *r = _ralloc();
	if (r != NULL) {
		r->type = Set;
		SetData *data = &(r->data.set_data);
		
		data->hash = hash;
		data->clock = clock;
		data->msg = msg;
		data->msg_len = msg_len;

		return r;
	}
	return NULL;
}

StoreRequest * done_request(hash_t hash, lgclock_t clock) {
	StoreRequest *r = _ralloc();
	if (r != NULL) {
		r->type = Done;
		DoneData *data = &(r->data.done_data);

		data->hash = hash;
		data->clock = clock;

		return r;
	}
	return NULL;
}
