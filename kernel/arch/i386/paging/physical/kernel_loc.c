#include <paging/physical/kernel_loc.h>
#include <stdint.h>

uintptr_t kloc_start = ~0;
uintptr_t kloc_end = 0;

void
ph_kloc_start_set(uintptr_t u) {
	kloc_start = u;
}

void
ph_kloc_end_set(uintptr_t u) {
	kloc_end = u;
}

uintptr_t
ph_kloc_start_get() {
	return kloc_start;
}

uintptr_t
ph_kloc_end_get(){
	return  kloc_end;
}
