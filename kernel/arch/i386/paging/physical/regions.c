#include <stddef.h>
#include <paging/physical/regions.h>
#include <multiboot/multiboot.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PH_REGION_COUNT 256
struct ph_region ph_all_regions[MAX_PH_REGION_COUNT];
ph_region_idx region_count = 0;

inline struct ph_region*
ph_get_region(ph_region_idx idx) {
	return &ph_all_regions[idx];
}

inline struct ph_region*
ph_region_array() {
	return ph_all_regions;
}

ph_region_idx
ph_new_region(struct ph_region p) {
	if (region_count >= MAX_PH_REGION_COUNT) return -1;

	size_t i = 0;
	for(i = 0; i < region_count; i++) {
		if (ph_all_regions[i].base >= p.base) break;
	}

	struct ph_region *dst = &ph_all_regions[i+1];
	struct ph_region *src = &ph_all_regions[i];
	memmove(dst, src, (region_count - i)*sizeof(struct ph_region));
	memcpy(&ph_all_regions[i], &p, sizeof(struct ph_region));
	region_count++;
	return i;
}

inline ph_region_idx
ph_new_region_params(uintptr_t base, size_t len, ph_region_flags_bitmask flags) {
	struct ph_region p;
	p.base = base;
	p.len = len;
	p.flags = flags;

	return ph_new_region(p);
}

inline ph_region_idx
ph_max_region_idx() {
	return MAX_PH_REGION_COUNT;
}

void
ph_delete_region(ph_region_idx i) {
	if(i + 1 == region_count) {
		region_count--;
		return;
	}
	if (i >= region_count) return;

	struct ph_region *dst = &ph_all_regions[i];
	struct ph_region *src = &ph_all_regions[i+1];

	memmove(dst, src, (region_count - i - 1)*sizeof(struct ph_region));
	region_count--;
}

size_t
ph_region_array_count() {
	return region_count;
}

bool
ph_is_region_contiguous(uintptr_t base, size_t size) {
	for (size_t i = 0; i < region_count; i++) {
		uintptr_t c_base = ph_get_region(i)->base;
		if (base + size <= c_base) break;
		if (base < c_base && c_base < base + size)
			return false;
	}
	return true;
}

bool
ph_is_region_free(uintptr_t base, size_t size) {
	if (!ph_is_region_contiguous(base, size)) return false;

	size_t i = 0;
	for(i = 0; i < region_count; i++) {
		if(ph_get_region(i)->base > base) break;
	}
	if (i != 0) i--;

	return ph_get_region(i)->flags == 0;
}
