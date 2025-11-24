DEFAULT_HOST!=../default_host.sh
HOST?=DEFAULT_HOST
HOST_ARCH!=../target-triplet-to-arch $(HOST)

CFLAGS?= -O2
ASFLAGS?= -O2
LDFLAGS?=
LIBS?=

DEST_D?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDE_D?=$(PREFIX)/include
LIB_D?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
ASFLAGS:=$(ASFLAGS)
LDFLAGS:=$(LDFLAGS)
LIBK_CFLAGS:=$(CFLAGS)

ARCH_D=arch/$(HOST_ARCH)

include $(ARCH_D)/make.config

CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
ASFLAGS:=$(ASFLAGS) $(ARCH_ASFLAGS)
LIBK_CFLAGS:=$(LIBK_CFLAGS) $(K_ARCH_CFLAGS)
LIBK_ASFLAGS:=$(ASFLAGS) $(K_ARCH_ASFLAGS)

FREE_OBJS= $(ARCH_FREE_OBJS)	\
		   stdio/printf.o		\
		   stdio/putchar.o		\
		   stdio/puts.o			\
		   stdlib/abort.o		\
		   string/memcmp.o		\
		   string/memcpy.o		\
		   string/memmove.o		\
		   string/memset.o		\
		   string/strlen.o		\

HOSTED_OBJS= $(ARCH_HOSTED_OBJS)

OBJS= $(FREE_OBJS) $(HOSTED_OBJS)

LIBK_OBJS=$(FREE_OBJS:.o=.libk.o)


BINARIES=libk.a

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libk.o .c .s

all: $(BINARIES)

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS)

.s.o:
	$(AS) -MD $< -o $@ $(ASFLAGS)

.c.libk.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS)

.s.libk.o:
	$(AS) -MD $< -o $@ $(LIBK_ASFLAGS)

clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs

install-headers:
	mkdir -p $(DEST_D)$(INCLUDE_D)
	cp -R --preserve=timestamps include/. $(DEST_D)$(INCLUDE_D)/.

install-libs: $(BINARIES)
	mkdir -p $(DEST_D)$(LIB_D)
	cp $(BINARIES) $(DEST_D)$(LIB_D)

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)
