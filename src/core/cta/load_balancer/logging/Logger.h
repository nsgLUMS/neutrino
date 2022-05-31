#ifndef LOGGER
#define LOGGER

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

// Lock to safely write into the file
extern pthread_mutex_t log_mutex;

void InitLogger(void);
void CloseLogger();
void WriteNumber(int N);
void WriteBytes(uint8_t *ptr, uint32_t length);
void WriteString(char *ptr, uint32_t length, char end);
void WriteDouble(double N);

#endif
