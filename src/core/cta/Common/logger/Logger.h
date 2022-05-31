#ifndef LOGGER
#define LOGGER

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

// Lock to safely write into the file
extern pthread_mutex_t log_mutex;

void InitLogger(void);
void CloseLogger();
void WriteLong(long N);
void WriteDouble(double N);
void WriteNumber(uint64_t N);
void WriteBytes(uint8_t *ptr, uint32_t length);
void WriteString(char *ptr, uint32_t length, char end);

#endif
