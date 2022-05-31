#ifndef _REQUESTS_H
#define _REQUESTS_H

#include "general.h"

typedef enum Type {
	Set,
	Done,
	Replay,
} Type;

typedef struct SetData {
	hash_t hash;
	lgclock_t clock;
	uint8_t const *msg;
	size_t msg_len;
} SetData;

typedef struct DoneData {
	hash_t hash;
	lgclock_t clock;
} DoneData;


typedef struct StoreRequest {
	Type type;
	union {
		SetData set_data;
		DoneData done_data;
	} data;
} StoreRequest;	

void rfree(StoreRequest *);

StoreRequest * set_request(hash_t, lgclock_t, uint8_t const *, size_t);

StoreRequest * done_request(hash_t, lgclock_t);
#endif
