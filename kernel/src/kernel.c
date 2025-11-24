#include <stdio.h>

#include <kernel/tty.h>

void
k_main() {
	tty_init();
	printf("Hello world!");
}
