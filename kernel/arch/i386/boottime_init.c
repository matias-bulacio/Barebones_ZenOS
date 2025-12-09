#include <multiboot/boottime.h>
#include <multiboot/multiboot.h>
#include <paging/physical/regions.h>
#include <elf/elf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
boottime_get_modules_ph_regions (multiboot_info_t *mbd) {
	if (!(mbd->flags & MULTIBOOT_INFO_MODS)) return;
	multiboot_module_t *mod_list = (multiboot_module_t*)mbd->mods_addr;
	size_t mod_count = mbd->mods_count;

	for(size_t i = 0; i < mod_count; i++) {
		multiboot_module_t *m = &mod_list[i];

		ph_new_region_params(m->mod_start, m->mod_end - m->mod_start, PH_REGION_MODULES);
	}
}

void
boottime_register_all_ph_regions (multiboot_info_t *mbd) {
	if(!(mbd->flags & MULTIBOOT_INFO_MEM_MAP)) {
		printf("Grub mmap invalid!");
		abort();
	}

	size_t i = 0;
	for (i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
		multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) (mbd->mmap_addr + i);

		ph_region_flags_bitmask f = 0;
		switch (mmap->type) {
			case MULTIBOOT_MEMORY_AVAILABLE:
				f = PH_REGION_AVAILABLE;
				break;
			case MULTIBOOT_MEMORY_RESERVED:
				f = PH_REGION_RESERVED_BIOS;
				break;
			case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
				f = PH_REGION_RECLAIMABLE_ACPI;
				break;
			case MULTIBOOT_MEMORY_BADRAM:
				f = PH_REGION_DEFECTIVE;
				break;
			case MULTIBOOT_MEMORY_NVS:
				f = PH_REGION_HIBERNATION_PRESERVE;
				break;
			default:
				f = PH_REGION_RESERVED_BIOS;
				break;
		}
		ph_new_region_params(mmap->addr_low, mmap->len_low, f);
	}
}

void
boottime_get_kernel_elf(multiboot_info_t *mbd) {
	if (!(mbd->flags & MULTIBOOT_INFO_ELF_SHDR)) return;

	size_t num		= mbd->u.elf_sec.num;
	size_t sec_size = mbd->u.elf_sec.size;

	Elf32_Shdr *elf_sec_table	= (Elf32_Shdr*) mbd->u.elf_sec.addr;
	Elf32_Shdr *current_sec		= elf_sec_table;

	for(size_t i = 0; i < num; i++, current_sec += sec_size) {
		uintptr_t	start_addr	= current_sec->sh_addr;
		size_t		size		= current_sec->sh_size;

		if (current_sec->sh_info == SHT_NOBITS || size == 0) {
			continue;
		}

		ph_new_region_params(start_addr, size, PH_REGION_KERNEL_ELF_SEGMENT);
	}
}

void
boottime_get_video_info (multiboot_info_t *mbd) {
	if(mbd->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO) {
		ph_new_region_params(mbd->framebuffer_addr, mbd->framebuffer_pitch*mbd->framebuffer_height, PH_REGION_VIDEO);
	}
}

void
boottime_register_multiboot_info_table(multiboot_info_t *mbd) {
	ph_new_region_params((uintptr_t) mbd, sizeof(multiboot_info_t), PH_REGION_MULTIBOOT_INFO_TABLE);

	if(mbd->flags & MULTIBOOT_INFO_CMDLINE) {
		ph_new_region_params(mbd->cmdline, strlen((char*)mbd->cmdline), PH_REGION_CMDLINE);
	}

	if(mbd->flags & MULTIBOOT_INFO_MODS) {
		ph_new_region_params(mbd->mods_addr, sizeof(multiboot_module_t) * mbd->mods_count, PH_REGION_MULTIBOOT_INFO_TABLE);
	}

	if(mbd->flags & MULTIBOOT_INFO_AOUT_SYMS) {
		multiboot_aout_symbol_table_t *aout = &mbd->u.aout_sym;
		ph_new_region_params(aout->addr, aout->tabsize + aout->strsize + 2*sizeof(uint32_t), PH_REGION_MULTIBOOT_INFO_TABLE);
	}

	if(mbd->flags & MULTIBOOT_INFO_ELF_SHDR) {
		multiboot_elf_section_header_table_t* elf = &mbd->u.elf_sec;
		Elf32_Shdr *elf_all = (Elf32_Shdr *)elf->addr;
		uintptr_t addr = elf->addr;
		size_t size = elf->size * elf->num;

		ph_new_region_params(addr, size, PH_REGION_MULTIBOOT_INFO_TABLE);

		uint32_t max_off = 0;
		for (size_t i = 0; i < elf->num; i++) {
			if(elf_all[i].sh_name > max_off) {
				max_off = elf_all[i].sh_name;
			}
		}

		size = max_off + strlen(&((char*)elf->shndx)[max_off]) + 1;

		ph_new_region_params(addr, size, PH_REGION_MULTIBOOT_INFO_TABLE);
	}

	if(mbd->flags & MULTIBOOT_INFO_MEM_MAP) {
		ph_new_region_params(mbd->mmap_addr, mbd->mmap_length, PH_REGION_MULTIBOOT_INFO_TABLE);
	}

	if(mbd->flags & MULTIBOOT_INFO_DRIVE_INFO) {
		ph_new_region_params(mbd->drives_addr, mbd->drives_length, PH_REGION_MULTIBOOT_INFO_TABLE);
	}
}

void
boottime_init(multiboot_info_t *mbd, uint32_t magic) {
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("Multiboot magic is wrong!\n");
		abort();
	}


	boottime_register_multiboot_info_table(mbd);

	boottime_register_all_ph_regions(mbd);
	boottime_get_kernel_elf(mbd);
	boottime_get_modules_ph_regions(mbd);
	boottime_get_video_info(mbd);
}
