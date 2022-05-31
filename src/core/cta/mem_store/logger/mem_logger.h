#ifndef _STORE_MEM_LOGGER_H
#define _STORE_MEM_LOGGER_H

#include <map>

#include "../general.h"

typedef struct LoggerElement {
	uint8_t *msg;
	size_t msg_len;
	double timestamp;
} Element;

typedef std::map<lgclock_t, Element*> User;
typedef std::map<hash_t, User> MemoryStore;

class MemLogger
{
	private:
		MemoryStore store;
		// Any element older than AGE will be considered old.
		// Unit is seconds.
		int age;

		size_t _rm(hash_t, lgclock_t);
		void MemLogger::_delete_user(hash_t);

	public:
		// void init(void);
		void LogMessage(hash_t, lgclock_t,  uint8_t const *, size_t);
		bool GetMessage(hash_t u_id, lgclock_t, uint8_t *);
		// User GetUser(hash_t);
		void DeleteMessage(hash_t, lgclock_t);
		size_t DeleteRange(hash_t, lgclock_t);
		void MemLogger::DeleteOld(unsigned int);
		void MemLogger::DeleteOldUser(hash_t);
		// void DeleteUser(hash_t);
		MemLogger(int);
		~MemLogger();
};

#endif
