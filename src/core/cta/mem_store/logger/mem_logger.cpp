#include<memory.h>

#include "mem_logger.h"
#include "../../Common/time/time.h"
#include "../../Common/globals.h"

Element* _ealloc(uint8_t const *msg, size_t len) {
	void *ptr = NULL;
	rte_mempool_get(global_mempool, &ptr);
	Element *e = (Element *) ptr;
	if (e == NULL) {
		// printf("Error! Memeory not allocated while creating an Element.");
		return NULL;
	}

	e->msg = msg;
	e->msg_len = len;	
	e->timestamp = TimeStampMicro();
	return e;
}

// Store message in the in-memory store.
void MemLogger::LogMessage(hash_t u_id, lgclock_t clock, const uint8_t *msg, size_t msg_len)
{
	Element* e = _ealloc(msg, msg_len);
	if (e == NULL) {
		return;
	}
	store[u_id][clock] = e;
}

size_t MemLogger::_rm(hash_t u_id, lgclock_t clock) {
	size_t size = store[u_id][clock]->msg_len + sizeof(LoggerElement);

	// TODO: Free msg, free(store[u_id][clock]->msg)
    rte_mempool_put(global_mempool, store[u_id][clock]);
	store[u_id].erase(clock);

	return size;
}

// Get single message for a user.
bool MemLogger::GetMessage(hash_t u_id, lgclock_t clock, uint8_t *msg)
{
	bool isMessageFound = false;

	if (store[u_id].find(clock) != store[u_id].end()) {
		msg = store[u_id][clock]->msg;
		isMessageFound = true;
	}
	return isMessageFound;
}

// Returns null value if element not found.
// User MemLogger::GetUser(hash_t u_id)
// {
// 	return store[u_id];
// }

// Delete single message for a particular user from the memory store.
void MemLogger::DeleteMessage(hash_t u_id, lgclock_t clock)
{
	User u = store[u_id];
	if (u.find(clock) != u.end()) {
		_rm(u_id, clock);
	}
}

// void _delete_range(User u, lgclock_t clock)
// {
// 	auto end = u.find(clock);
// 
// 	for (auto it = u.begin(); it != end; it++) {
// 		_rm(u_id, it->first);
// 	}
// }

// Delete messages upto the logical clock for a signle user.
size_t MemLogger::DeleteRange(hash_t u_id, lgclock_t clock)
{
	User u = store[u_id];
	// +2 to also delete the entry with the given clock.
	auto end = u.lower_bound(clock + 2);
	size_t size = 0;

	for (auto it = u.begin(); it != end; it++) {
		size += _rm(u_id, it->first);
	}

	return size;
}

// Delete all messages for a user.
void MemLogger::_delete_user(hash_t u_id)
{
	auto it = store.find(u_id);
	if (it != store.end()) {
		User u = it->second;
		DeleteRange(u_id, u.rbegin()->first);
	}
}

bool _is_old(User u, unsigned int age) {
	Element *e = u.rbegin()->second;
	if (MicroToSecond(e->timestamp) > age)
		return true;

	return false;
}

void MemLogger::DeleteOldUser(hash_t u_id) {
	User u = store[u_id];
	if (_is_old(u, age))
		_delete_user(u_id);
}

void MemLogger::DeleteOld(unsigned int age) {
	for (auto it = store.begin(); it != store.end(); it++)
		DeleteOldUser(it->first);
}

MemLogger::MemLogger(int age)
{
	this->age = age;
}


MemLogger::~MemLogger()
{
	// Clean up, delete everything.
 	// for (auto it = store.begin(); it != store.end(); it++)
	// 	_delete_user(it->first);
}
