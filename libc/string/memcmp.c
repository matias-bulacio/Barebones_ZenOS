#include <stddef.h>
#include <stdint.h>
#include <string.h>

int
memcmp(const void* aptr, const void* bptr, size_t size){
	uint8_t *a = (uint8_t*)aptr;
	uint8_t *b = (uint8_t*)bptr;

	for(size_t i = 0; i < size; i++) {
		if(a[i] < b[i]) return -1;
		if(a[i] > b[i]) return  1;
	}
	return 0;
}
