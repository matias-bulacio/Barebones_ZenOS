#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(__linux__)
#error "Use a cross-compiler!"
#endif
#if !defined(__i386__)
#error "Need an ix86 compiler"
#endif

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t
vga_entry_color (enum vga_color fg, enum vga_color bg) {
	return bg << 4 | fg;
}

static inline uint16_t
vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) color << 8 | (uint16_t) uc;
}

size_t
strlen(const char *str) {
	size_t i = 0;
	while(str[i] != 0) i++;
	return i;
}

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BUFFER 0xB8000

size_t term_cursor;

uint8_t term_color;
uint16_t *term_buff = (uint16_t*)VGA_BUFFER;

void term_clear(){
	size_t old_term_cursor = term_cursor;

	for(term_cursor = 0; term_cursor < VGA_WIDTH * VGA_HEIGHT; term_cursor++) {
		term_buff[term_cursor] = vga_entry(' ', term_color);
	}

	term_cursor = old_term_cursor;
}

void
term_init(){
	term_cursor = 0;
	term_color = vga_entry_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
}

void
term_set_cursor(size_t x, size_t y) {
	term_cursor = y * VGA_WIDTH + x;
}

bool
term_cursor_inc() {
	term_cursor = (term_cursor + 1) % (VGA_WIDTH * VGA_HEIGHT);

	return term_color == 0;
}

void
term_putchar(char c) {
	term_buff[term_cursor] = vga_entry((unsigned char) c, term_color);
	term_cursor_inc();
}

void
term_write(const char *data, size_t size){
	for (size_t i = 0; i < size; i++) {
		term_putchar(data[i]);
	}
}

inline void
term_write_str(const char *str) {
	term_write(str, strlen(str));
}

void
k_main() {
	term_init();
	term_write_str("Hello world!\n");
}
