#include <stdio.h>
#include <kernel/tty.h>
#include <paging/physical/regions.h>
#include <stdlib.h>
#include <stdbool.h>

void
k_main() {
	//tty_init();
	ph_region_id i = ph_head_region_id();
	if (i == -1) {
		printf("Invalid HEAD ID");
		abort();
	}
	struct ph_region r = ph_get_region(i);
	printf("All regions")
	while(true) {
		printf("Start: %x\tEnd: %x\n", r.base, r.base + r.len);
		if(r.next != NULL) r = *r.next;
		else break;
	}

}
