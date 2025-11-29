#include <stdio.h>
#include <kernel/tty.h>
#include <paging/physical/regions.h>
#include <stdlib.h>
#include <stdbool.h>

void
k_main() {
	//tty_init();
	size_t count = ph_region_array_count();
	for(size_t i = 0; i < count; i++) {
		struct ph_region *r = ph_get_region(i);
		printf("Start: %x\tEnd: %x\tFlags: %x\n", r->base, r->base + r->len, r->flags);
	}
}
