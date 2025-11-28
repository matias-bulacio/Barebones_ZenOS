#include <stddef.h>
#include <paging/physical/regions.h>
#include <multiboot/multiboot.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PH_REGION_COUNT 256
struct ph_region ph_all_regions[MAX_PH_REGION_COUNT];

ph_region_id region_count = 0;

struct ph_region ph_get_region(ph_region_id id) {
	return ph_all_regions[id];
}

ph_region_id
ph_new_region(struct ph_region p) {
	if (region_count >= MAX_PH_REGION_COUNT) {
		return -1;
	}
	if (region_count == 0) {
		p.next = NULL;
	} else {
		p.next = &ph_all_regions[region_count - 1];
	}
	ph_all_regions[region_count++] = p;
	return region_count - 1;
}

ph_region_id
ph_new_region_params(uintptr_t base, size_t len) {
	struct ph_region p;
	p.base = base;
	p.len = len;

	return ph_new_region(p);
}

ph_region_id
ph_last_region_id() {
	return region_count - 1;
}

ph_region_id
ph_max_region_id() {
	return MAX_PH_REGION_COUNT;
}

