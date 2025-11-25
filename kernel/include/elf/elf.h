#ifndef _KERNEL_ELF_H
#define _KERNEL_ELF_H

#include <stdint.h>

typedef struct {
	uint32_t	sh_name;
	uint32_t	sh_type;
	uint32_t	sh_flags;
	uint32_t	sh_addr;
	uint32_t	sh_offset;
	uint32_t	sh_size;
	uint32_t	sh_link;
	uint32_t	sh_info;
	uint32_t	sh_addralign;
	uint32_t	sh_entsize;
} Elf32_Shdr;

#endif // _KERNEL_ELF_H
