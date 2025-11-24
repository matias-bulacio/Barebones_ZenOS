#include <kernel/tty.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "vga.h"

size_t tty_cursor;

uint8_t tty_color;
uint16_t *tty_buff = (uint16_t*)VGA_BUFFER;

#define VGA_LAST_LINE VGA_WIDTH*(VGA_HEIGHT-1)

#ifndef TTY_TAB_WIDTH
#define TTY_TAB_WIDTH 4
#endif

bool
tty_scroll_if_needed(size_t lines, size_t new_cursor);

bool
tty_cursor_inc();

void
tty_init() {
	tty_cursor = 0;
	tty_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
}

void
tty_putchar(char c) {
	if (c == '\n') {
		tty_cursor += VGA_WIDTH - tty_cursor % VGA_WIDTH;
		tty_scroll_if_needed(1, VGA_LAST_LINE);
		return;
	}

	if (c == '\t') {
		size_t old_tty_cursor = tty_cursor;
		tty_cursor += TTY_TAB_WIDTH - tty_cursor % TTY_TAB_WIDTH;

		for (size_t i = old_tty_cursor; i < tty_cursor && i < VGA_END_IDX; i++) {
			tty_buff[i] = vga_entry(' ', tty_color);
		}

		tty_scroll_if_needed(1, VGA_LAST_LINE);
		return;
	}

	tty_buff[tty_cursor] = vga_entry(' ', tty_color);
	tty_cursor_inc();
}

void
tty_write(const char *data, size_t size) {
	for (size_t i = 0; i < size; i++){
		tty_putchar(data[i]);
	}
}

inline void
tty_write_string(const char *str){
	tty_write(str, strlen(str));
}

void
tty_clear(){
	for(size_t i = 0; i < VGA_END_IDX; i++) {
		tty_buff[i] = vga_entry(' ', tty_color);
	}
}

void
tty_scroll(size_t lines) {
	if (lines >= VGA_HEIGHT) {
		tty_clear();
		return;
	}

	for (size_t i = 0; i < VGA_HEIGHT-lines; i++) {
		memcpy(&tty_buff[i*VGA_WIDTH], &tty_buff[(i+lines)*VGA_WIDTH], VGA_WIDTH*2);
	}

	for (size_t i = VGA_WIDTH*(VGA_HEIGHT-lines); i<VGA_END_IDX; i++) {
		tty_buff[i] = vga_entry(' ', tty_color);
	}
}

void
tty_set_cursor(size_t x, size_t y) {
	tty_cursor = y*VGA_WIDTH + x;
}

void
tty_set_color(uint8_t color) {
	tty_color = color;
}

bool
tty_scroll_if_needed(size_t count, size_t new_cursor){
	bool ret_val = false;
	if(tty_cursor >= VGA_END_IDX) {
		tty_scroll(count);
		tty_cursor = new_cursor;
		ret_val = true;
	}
	return ret_val;
}

bool
tty_cursor_inc() {
	tty_cursor = (tty_cursor + 1);
	bool ret_val = tty_scroll_if_needed(1, VGA_LAST_LINE);

	return ret_val;
}
