#include "encoder_decoder.h"

/* This function converts a uint8_t* array to a void* array. The void* array is allocated using malloc, please remember to free it, once used. */
void* Uint8ToVoidArray(const uint8_t *buffer, int size) {
    void *regen = malloc(size);
    void *ptr = regen;
    for (int i = 0; i < size; i++)
    {
        const char *curr = (buffer + i);
        *(char *)ptr++ = *(uint8_t *)((void *)curr);
    }
    return regen;
}

// This function typecasts a void* array to uint8_t array and displays it.
void VoidToUint8Array(void * buffer, int size) {
    printf("{");
	for (int i = 0; i < size; i++) {
		printf("%u, ",*((uint8_t*) (buffer + i)));
	}
	printf("}\n");
}

// This function converts a hex number to base-10 integer.
int HexToInt(char* input) {
	return (int) strtol(input, NULL, 16);
}
