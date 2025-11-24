#include <stdio.h>
#include <stdlib.h>

void abort(void) {
#if defined(__is_libk)
	printf("kernel: panic: abort()\n");
#else
	printf("abort()\n");
#endif
	while (1);
}
