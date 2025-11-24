DEFAULT_HOST!=../default_host.sh
HOST?=DEFAULT_HOST
HOST_ARCH!=../target-triplet-to-arch $(HOST)

K_NAME?=ZenOS-$(HOST_ARCH).kernel

CFLAGS?= -O2
ASFLAGS?= -O2
LDFLAGS?=
LIBS?=

DEST_D?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOT_D?=$(EXEC_PREFIX)/boot
INCLUDE_D?=$(PREFIX)/include

CFLAGS:= $(CFLAGS) -ffreestanding -Wall -Wextra
ASFLAGS:=$(ASFLAGS)
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCH_D=arch/$(HOST_ARCH)

include $(ARCH_D)/make.config

CFLAGS:=$(CFLAGS) $(K_ARCH_CFLAGS)
LDFLAGS:=$(LDFLAGS) $(K_ARCH_LDFLAGS)
ASFLAGS:=$(ASFLAGS) $(K_ARCH_ASFLAGS)
LIBS:=$(LIBS) $(K_ARCH_LIBS)

K_OBJS = $(K_ARCH_OBJS)\
		 src/kernel.o

OBJS = $(K_OBJS)

LINK_LIST = $(OBJS) $(LIBS)

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .s

all: ZenOS.kernel

$(K_NAME): $(OBJS) $(ARCH_D)/linker.ld
	$(CC) -T $(ARCH_D)/linker.ld -o $@ $(CFLAGS) $(LDFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot $@

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS)

.s.o:
	$(AS) -MD $< -o $@ $(ASFLAGS)

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
