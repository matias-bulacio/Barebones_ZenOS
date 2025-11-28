#ifndef _KERNEL_PAGING_PHYSICAL_REGIONS_H
#define _KERNEL_PAGING_PHYSICAL_REGIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint32_t ph_region_id;

struct ph_region {
	uintptr_t base;
	size_t len;
	struct ph_region *next;
};

ph_region_id
ph_new_region_params(uintptr_t, size_t);

struct ph_region
ph_get_region(ph_region_id id);

ph_region_id
ph_new_region(struct ph_region p);

ph_region_id
ph_head_region_id();

ph_region_id
ph_max_region_id();

#endif // _KERNEL_PAGING_PHYSICAL_REGIONS_H
