#include <multiboot/boottime.h>
#include <multiboot/multiboot.h>
#include <paging/physical/pages.h>
#include <paging/physical/kernel_loc.h>
#include <elf/elf.h>
#include <stdio.h>
#include <stdlib.h>

void
boottime_register_all_ph_pages (multiboot_info_t *mbd) {
	if(!((mbd->flags >> 6) & 1 )) {
		printf("Grub mmap invalid!");
		abort();
	}

	size_t i = 0;
	for (i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
		multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) (mbd->mmap_addr + i);

		if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
			ph_new_page_params(mmap->addr_low, mmap->len_low);
		}
	}
}

void
boottime_get_kernel_loc(multiboot_info_t *mbd) {
	if (mbd->flags & MULTIBOOT_INFO_ELF_SHDR) {
		size_t num = mbd->u.elf_sec.num;
		size_t sec_size = mbd->u.elf_sec.size;
		char *shndx = (char *)mbd->u.elf_sec.shndx;

		Elf32_Shdr *elf_sec_table = (Elf32_Shdr*) mbd->u.elf_sec.addr;
		Elf32_Shdr *current_sec = elf_sec_table;
		uint32_t min_k_loc = UINTPTR_MAX;
		uint32_t max_k_loc = 0;
		for(size_t i = 0; i < num; i++, current_sec += sec_size) {
			if(min_k_loc > current_sec->sh_addr) min_k_loc = current_sec->sh_addr;
			if(max_k_loc < current_sec->sh_addr + current_sec->sh_size) max_k_loc = current_sec->sh_addr + current_sec->sh_size;
		}
		ph_kloc_start_set((uintptr_t)min_k_loc);
		ph_kloc_end_set((uintptr_t)max_k_loc);
	}
}

void
boottime_init(multiboot_info_t *mbd, uint32_t magic) {
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("Multiboot magic is wrong!\n");
		abort();
	}

	boottime_register_all_ph_pages(mbd);
	boottime_get_kernel_loc(mbd);


}
