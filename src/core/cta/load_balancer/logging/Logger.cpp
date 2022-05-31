#include "Logger.h"

FILE *fp;
/* open the file for writing*/
pthread_mutex_t log_mutex;

// Initialize circular buffer.
void InitLogger()
{
	pthread_mutex_init(&log_mutex, NULL);
	fp = fopen("LB_Logs.txt", "w");
	fprintf(fp, "Initializting Load Balancer ... \n");
	fflush(fp);
}

void CloseLogger(void)
{
	fclose(fp);
}

void WriteDouble(double N) {
    pthread_mutex_lock(&log_mutex);
    fprintf(fp, "%.3f\n", N);
    fflush(fp);
    pthread_mutex_unlock(&log_mutex);
}

void WriteBytes(uint8_t *ptr, uint32_t length)
{
	uint32_t index;

	pthread_mutex_lock(&log_mutex);
	for (index = 0; index < length; index++)
	{
		fprintf(fp, "%u, ", *ptr++);
	}

	fprintf(fp, "\n");
	fflush(fp);
	pthread_mutex_unlock(&log_mutex);
}

void WriteString(char *ptr, uint32_t length, char end)
{
	uint32_t index;
	pthread_mutex_lock(&log_mutex);
	for (index = 0; index < length; index++)
	{
		fprintf(fp, "%c", *ptr++);
	}

	fprintf(fp, "%c", end);
	fflush(fp);
	pthread_mutex_unlock(&log_mutex);
}

void WriteNumber(int N)
{
	pthread_mutex_lock(&log_mutex);
	fprintf(fp, "%d\n", N);
	fflush(fp);
	pthread_mutex_unlock(&log_mutex);
}
