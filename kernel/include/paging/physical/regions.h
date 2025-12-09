#ifndef _KERNEL_PAGING_PHYSICAL_REGIONS_H
#define _KERNEL_PAGING_PHYSICAL_REGIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint32_t ph_region_idx;

typedef uint32_t ph_region_flags_bitmask;

#define PH_REGION_AVAILABLE				(ph_region_flags_bitmask)0x001 //   1
#define PH_REGION_RESERVED_BIOS			(ph_region_flags_bitmask)0x002 //   2
#define PH_REGION_HIBERNATION_PRESERVE	(ph_region_flags_bitmask)0x004 //   4
#define PH_REGION_KERNEL_ELF_SEGMENT	(ph_region_flags_bitmask)0x008 //   8
#define PH_REGION_MODULES				(ph_region_flags_bitmask)0x010 //  16
#define PH_REGION_RECLAIMABLE_ACPI		(ph_region_flags_bitmask)0x020 //  32
#define PH_REGION_VIDEO					(ph_region_flags_bitmask)0x040 //  64
#define PH_REGION_DEFECTIVE				(ph_region_flags_bitmask)0x080 // 128
#define PH_REGION_MULTIBOOT_INFO_TABLE	(ph_region_flags_bitmask)0x100 // 256
#define PH_REGION_CMDLINE				(ph_region_flags_bitmask)0x200 // 512

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

bool
ph_is_region_contiguous(uintptr_t base, size_t size);

bool
ph_is_region_free(uintptr_t base, size_t size);

#endif // _KERNEL_PAGING_PHYSICAL_REGIONS_H
