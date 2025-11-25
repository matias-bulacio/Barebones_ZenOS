#ifndef _KERNEL_MULTIBOOT_BOOTTIME_H
#define _KERNEL_MULTIBOOT_BOOTTIME_H

#include <multiboot/multiboot.h>
#include <stdint.h>

void
boottime_init(multiboot_info_t *mbd, uint32_t magic);

#endif // _KERNEL_MULTIBOOT_BOOTTIME_H
