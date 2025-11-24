#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

void
tty_init();

void
tty_putchar(char c);

void
tty_write(const char *data, size_t size);

void
tty_write_string(const char *str);

void
tty_clear();

void
tty_scroll(size_t lines);

void
tty_set_cursor(size_t x, size_t y);

void
tty_set_color(uint8_t color);

#endif
