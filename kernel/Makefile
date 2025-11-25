DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOST_ARCH!=../target-triplet-to-arch.sh $(HOST)

K_NAME?=zenos.kernel

#CPPFLAGS mean Compiler Pre-Processor flags

CFLAGS?= -O2
CPPFLAGS?=  
ASFLAGS?= -O2
LDFLAGS?=
LIBS?=

DEST_D?=$(SYSROOT)
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOT_D?=$(EXEC_PREFIX)/boot
INCLUDE_D?=$(PREFIX)/include

CFLAGS:= $(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
ASFLAGS:=$(ASFLAGS)
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCH_D=arch/$(HOST_ARCH)

include $(ARCH_D)/make.config

CFLAGS:=$(CFLAGS) $(K_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(K_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(K_ARCH_LDFLAGS)
ASFLAGS:=$(ASFLAGS) $(K_ARCH_ASFLAGS)
LIBS:=$(LIBS) $(K_ARCH_LIBS)

K_OBJS = $(K_ARCH_OBJS)\
		 src/kernel.o

OBJS = $(K_OBJS)

LINK_LIST = $(OBJS) $(LIBS)

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .s

all: $(K_NAME)

$(K_NAME): $(OBJS) $(ARCH_D)/linker.ld
	$(CC) -T $(ARCH_D)/linker.ld -o $@ $(CFLAGS) $(LDFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot $@

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

.s.o:
	$(AS) $< -o $@ $(ASFLAGS) $(CPPFLAGS) -MD ${@:.o=.d}

clean:
	rm -f $(K_NAME)
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel

install-headers:
	mkdir -p $(DEST_D)$(INCLUDE_D)
	cp -R --preserve=timestamps include/. $(DEST_D)$(INCLUDE_D)/.

install-kernel: $(K_NAME)
	mkdir -p $(DEST_D)$(BOOT_D)
	cp $(K_NAME) $(DEST_D)$(BOOT_D)

-include $(OBJS:.o=.d)
