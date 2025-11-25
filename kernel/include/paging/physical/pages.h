#ifndef _KERNEL_PAGING_PHYSICAL_PAGES_H
#define _KERNEL_PAGING_PHYSICAL_PAGES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint32_t ph_page_id;

struct ph_page {
	uintptr_t base;
	size_t len;
	struct ph_page *next;
};

ph_page_id
ph_new_page_params(uintptr_t, size_t);

struct ph_page
ph_get_page(ph_page_id id);

ph_page_id
ph_new_page(struct ph_page p);

ph_page_id
ph_last_page_id();

ph_page_id
ph_max_page_id();

#endif // _KERNEL_PAGING_PHYSICAL_PAGES_H
