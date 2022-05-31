#include "Logger.h"

// Initialize circular buffer.
FILE* InitLogger(char name[])
{
	return fopen(name, "w");

}

void CloseLogger(FILE *fp)
{
	fclose(fp);
}

void WriteBytes(uint8_t *ptr, uint32_t length, FILE *fp)
{
	uint32_t index;

	for (index = 0; index < length; index++)
	{
		fprintf(fp, "%u, ", *ptr++);
	}

	fprintf(fp, "\n");
	fflush(fp);
}

void WriteString(char *ptr, uint32_t length, char end, FILE *fp)
{
	uint32_t index;
	for (index = 0; index < length && *ptr != end; index++)
	{
		fprintf(fp, "%c", *ptr++);
	}

	fprintf(fp, "%c", end);
	fflush(fp);
}

void WriteNumber(uint64_t N, FILE *fp)
{
	fprintf(fp, "%u\n", N);
	fflush(fp);
}

void WriteDouble(double N, FILE *fp)
{
	fprintf(fp, "%f\n", N);
	fflush(fp);
}

void WriteLong(long N, FILE *fp)
{
	fprintf(fp, "%ld\n", N);
	fflush(fp);
}
