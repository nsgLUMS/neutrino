// These functions do the main processing of the memory store
// using the internal of memory store, namely logger.

#include "store.h"

#include <rte_malloc.h>

#include "../Common/logger/Logger.h"
#include "../Common/time/time.h"

#include "logger/mem_logger.h"
#include "../Common/globals.h"

// typedef struct {
// 	double time;
// 	int type;
// 	ssize_t size;
// } LoggerSizeElement;

// static LoggerSizeElement *list;
// static size_t size = ((200000 * 8) + 1) * sizeof(LoggerSizeElement);
// static int counter = 0;
// static size_t max_size = 0;

// void set_logger_element(ssize_t size, int type, LoggerSizeElement *e) {
// 	e->time = TimeStampMicro() - start_time;
// 	e->type = type;
// 	e->size = size;

// 	if (type == 0 && size > max_size) {
// 			max_size = size;
// 	}
// }

// static ssize_t *list;
// static size_t list_size = 50000000;
// static uint64_t counter = 0;

// static double start_time = 0;;
// static double end_time;

inline static void _process_set(SetData *r, MemLogger &logger) {
	// set_logger_element(list[counter - 1].size + r->msg_len + sizeof(LoggerElement), 0, list + counter);
	// counter++;

	// list[counter] = r->msg_len + sizeof(SetData) + list[counter - 1];
	// counter++;

	rte_mempool_put(global_mempool, (void *)(r->msg));
	// Uncocpfnt the following line to enable the actual pkt logging.
	// This was disabled because the processing rate of pkt logging was
	// way lower than the dequeue threads and CPFs. Future plan for this
	// is to use dpdk hash library rather than C++ map.
	// logger.LogMessage(r->hash, r->clock, r->msg, r->msg_len);
}

inline static void _process_done(DoneData *r, MemLogger &logger) {
	// list[counter] = list[counter - 1] - 2428;
	// counter++;
	// size_t size = logger.DeleteRange(r->hash, r->clock);

	// set_logger_element(list[counter - 1].size - size, 1, list + counter);
	// counter++;
}

inline static void _process_request(StoreRequest *r) {
	static MemLogger logger(10);
	

	switch(r->type) {
		case Set:
			_process_set(&r->data.set_data, logger);
			break;
		case Done:
			_process_done(&r->data.done_data, logger);
			break;
		case Replay:
			break;
	}
	rfree(r);
}

void init(rte_ring *ring) {
	printf("Starting a separate thread for memory store on core %d\n", rte_lcore_id());

	// list = (ssize_t *)rte_malloc(NULL, list_size, 0);
	// if (list == NULL) {
	// 	printf("Cannot get the memory for list in memory store.\n");
	// 	return;
	// }

	while (!force_quit) {
		void *ptr;
		// TODO: Dequeue in bulk might help here.
		int res = rte_ring_dequeue(ring, &ptr);
		if (res != 0)
			continue;

		// if (start_time == 0) {
		// 	start_time = TimeStampMicro();
		// }

		_process_request((StoreRequest *) ptr);
		// end_time = TimeStampMicro();
	}

	// printf("Store rate: %0.6f pps\n", counter / ((end_time - start_time) * 1e-6));

 	// for(uint64_t i = 0; i < counter; i++) {
	// 	 WriteLong(list[i]);
 	// 	// char str[64];
 	// 	// sprintf(str, "%f %d %lu\n", list[i].time, list[i].type, list[i].size); 
 	// 	// WriteString(str, 64, '\n');
 	// }

	// rte_free(list);
}
