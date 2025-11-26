#include <stdio.h>

#include <kernel/tty.h>
#include <paging/physical/pages.h>

void
k_main() {
	//tty_init();
	struct ph_page p;

	for (p = ph_get_page(ph_last_page_id()); p.next != NULL; p = *p.next) {
		printf("%x\t%x\n", p.base, p.len);
	}

	printf("K_START: %x\n", ph_kloc_start_get());
	printf("K_END  : %x\n", ph_kloc_end_get(  ));

}
