#include <stddef.h>
#include <paging/physical/pages.h>
#include <multiboot/multiboot.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PH_PAGE_COUNT 256
struct ph_page ph_all_pages[MAX_PH_PAGE_COUNT];

ph_page_id page_count = 0;

struct ph_page ph_get_page(ph_page_id id) {
	return ph_all_pages[id];
}

ph_page_id
ph_new_page(struct ph_page p) {
	if (page_count >= MAX_PH_PAGE_COUNT) {
		return -1;
	}
	if (page_count == 0) {
		p.next = NULL;
	} else {
		p.next = &ph_all_pages[page_count - 1];
	}
	ph_all_pages[page_count++] = p;
	return page_count - 1;
}

ph_page_id
ph_new_page_params(uintptr_t base, size_t len) {
	struct ph_page p;
	p.base = base;
	p.len = len;

	return ph_new_page(p);
}

ph_page_id
ph_last_page_id() {
	return page_count - 1;
}

ph_page_id
ph_max_page_id() {
	return MAX_PH_PAGE_COUNT;
}

