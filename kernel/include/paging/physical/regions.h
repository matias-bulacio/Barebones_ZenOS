#ifndef _KERNEL_PAGING_PHYSICAL_REGIONS_H
#define _KERNEL_PAGING_PHYSICAL_REGIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint32_t ph_region_idx;

typedef uint32_t ph_region_flags_bitmask;

#define PH_REGION_RECLAIMABLE_ACPI		(ph_region_flags_bitmask)0b00000001
#define PH_REGION_RESERVED_BIOS			(ph_region_flags_bitmask)0b00000010
#define PH_REGION_DEFECTIVE				(ph_region_flags_bitmask)0b10000000
#define PH_REGION_HIBERNATION_PRESERVE	(ph_region_flags_bitmask)0b00000100
#define PH_REGION_KERNEL_ELF_SEGMENT	(ph_region_flags_bitmask)0b00001000
#define PH_REGION_MODULES				(ph_region_flags_bitmask)0b00010000

struct ph_region {
	uintptr_t base;
	size_t len;
	ph_region_flags_bitmask flags;
};

ph_region_idx
ph_new_region_params(uintptr_t, size_t, ph_region_flags_bitmask);

struct ph_region*
ph_get_region(ph_region_idx idx);

ph_region_idx
ph_new_region(struct ph_region p);

void
ph_delete_region(ph_region_idx i);

ph_region_idx
ph_max_region_idx();

struct ph_region*
ph_region_array();

size_t
ph_region_array_count();

#endif // _KERNEL_PAGING_PHYSICAL_REGIONS_H
