#ifndef _STORE_H
#define _STORE_H

#include <rte_ring.h>

#include "general.h"
#include "requests.h"

void init(rte_ring *);

#endif
