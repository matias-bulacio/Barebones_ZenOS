#include <stdio.h>

#ifdef __is_libk
#include <kernel/tty.h>
#endif

int
putchar(int c) {

#ifdef __is_libk
	char ic = (char) c;
	tty_write(&ic, sizeof(ic));
#else
	// TODO
#endif

	return c;
}
