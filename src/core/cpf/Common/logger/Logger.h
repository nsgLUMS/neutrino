#ifndef LOGGER
#define LOGGER

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

// Lock to safely write into the file
extern pthread_mutex_t log_mutex;

FILE*  InitLogger(char name[]);
void CloseLogger(FILE *fp);
void WriteLong(long N, FILE *fp);
void WriteDouble(double N, FILE *fp);
void WriteNumber(uint64_t N, FILE *fp);
void WriteBytes(uint8_t *ptr, uint32_t length, FILE *fp);
void WriteString(char *ptr, uint32_t length, char end, FILE *fp);

#endif
