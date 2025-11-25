#ifndef _KERNEL_PAGING_PHYSICAL_KERNEL_LOC
#define _KERNEL_PAGING_PHYSICAL_KERNEL_LOC

#include <stdint.h>

void
ph_kloc_start_set(uintptr_t);

void
ph_kloc_end_set(uintptr_t);

uintptr_t
ph_kloc_start_get();

uintptr_t
ph_kloc_end_get();

#endif // _KERNEL_PAGING_PHYSICAL_KERNEL_LOC
