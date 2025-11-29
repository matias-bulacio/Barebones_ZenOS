#include <multiboot/boottime.h>
#include <multiboot/multiboot.h>
#include <paging/physical/regions.h>
#include <elf/elf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void
boottime_register_all_ph_regions (multiboot_info_t *mbd) {
	if(!((mbd->flags >> 6) & 1 )) {
		printf("Grub mmap invalid!");
		abort();
	}

	size_t i = 0;
	for (i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
		multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) (mbd->mmap_addr + i);

		if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
			ph_new_region_params(mmap->addr_low, mmap->len_low, 0);
		}
	}
}

void
boottime_get_kernel_elf(multiboot_info_t *mbd) {
	if (!(mbd->flags & MULTIBOOT_INFO_ELF_SHDR)) return;

	size_t num		= mbd->u.elf_sec.num;
	size_t sec_size = mbd->u.elf_sec.size;
	char *shndx		= (char *)mbd->u.elf_sec.shndx;

	Elf32_Shdr *elf_sec_table	= (Elf32_Shdr*) mbd->u.elf_sec.addr;
	Elf32_Shdr *current_sec		= elf_sec_table;

	for(size_t i = 0; i < num; i++, current_sec += sec_size) {
		uintptr_t	start_addr	= current_sec->sh_addr;
		size_t		size		= current_sec->sh_size;
		uintptr_t	end_addr	= start_addr + size;
		char	   *name		= &shndx[current_sec->sh_name];

		if (current_sec->sh_info == SHT_NOBITS || size == 0) {
			continue;
		}

		//printf("Start: %x\tSize: %x\tEnd: %x\tName: %s\n", start_addr, size, end_addr, name);

		ph_new_region_params(start_addr, size, PH_REGION_KERNEL_ELF_SEGMENT);
	}
}

void
boottime_init(multiboot_info_t *mbd, uint32_t magic) {
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("Multiboot magic is wrong!\n");
		abort();
	}

	boottime_register_all_ph_regions(mbd);
	boottime_get_kernel_elf(mbd);


}
