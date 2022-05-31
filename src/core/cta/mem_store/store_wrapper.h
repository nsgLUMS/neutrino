#ifndef _STORE_WRAPPER_H
#define _STORE_WRAPPER_H

#include "general.h"

void init_store (void *args);

void save(hash_t hash, lgclock_t clock, uint8_t const *msg, size_t msg_len);

void done(hash_t hash, lgclock_t clock);

void synced(hash_t hash);

void replay(hash_t hash);

#endif
