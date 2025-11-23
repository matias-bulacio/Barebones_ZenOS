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
#define VGA_LAST_LINE (VGA_HEIGHT-1)*VGA_WIDTH

#define TERM_TAB_WIDTH 4

size_t term_cursor;

uint8_t term_color;
uint16_t *term_buff = (uint16_t*)VGA_BUFFER;

void
term_clear(){
	size_t old_term_cursor = term_cursor;

	for(term_cursor = 0; term_cursor < VGA_WIDTH * VGA_HEIGHT; term_cursor++) {
		term_buff[term_cursor] = vga_entry(' ', term_color);
	}

	term_cursor = old_term_cursor;
}

size_t
memcpy(void *dest, void *src, size_t n) {
	size_t i = 0;
	for(i = 0; i < n; i++){
		((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
	}
	return i;
}

void
uitoa_hex(uint32_t value, char* str) {
    char *p = str;
    char *p1, *p2;

    do {
        int digit = value % 16;
        *p++ = (digit < 10) ? (digit + '0') : (digit - 10 + 'A');
        value /= 16;
    } while (value != 0);

    *p = '\0';

    for (p1 = str, p2 = p - 1; p1 < p2; p1++, p2--) { // Reverse
        char temp = *p1;
        *p1 = *p2;
        *p2 = temp;
    }
}

void
term_init(){
	term_cursor = 0;
	term_color = vga_entry_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
}

size_t
term_cursor_pos(size_t x, size_t y) {
	return y * VGA_WIDTH + x;
}

void
term_scroll(size_t lines) {
	size_t old_term_cursor = term_cursor;

	if(lines >= VGA_HEIGHT) {
		term_clear();
		goto ret;
	}

	for (size_t i = 0; i < VGA_HEIGHT-lines; i++) {
		memcpy(&term_buff[i*VGA_WIDTH], &term_buff[(i+lines)*VGA_WIDTH], VGA_WIDTH*2);
	}

	for (size_t i = VGA_WIDTH*(VGA_HEIGHT-lines); i<VGA_WIDTH*VGA_HEIGHT; i++) {
		term_buff[i] = vga_entry(' ', term_color);
	}

ret:
	term_cursor = old_term_cursor;
	return;
}

bool
term_scroll_if_needed(size_t count, size_t new_cursor){
	bool ret_val = false;
	if(term_cursor >= VGA_WIDTH * VGA_HEIGHT) {
		term_scroll(count);
		term_cursor = new_cursor;
		ret_val = true;
	}
	return ret_val;
}


bool
term_cursor_inc() {
	term_cursor = (term_cursor + 1);
	bool ret_val = term_scroll_if_needed(1, VGA_LAST_LINE);

	return ret_val;
	//return false;
}

void
term_putchar(char c) {
	if(c == '\n') {
		term_cursor += VGA_WIDTH - term_cursor % VGA_WIDTH;
		term_scroll_if_needed(1, VGA_LAST_LINE);
		return;
	}

	if(c == '\t') {
		size_t old_term_cursor = term_cursor;
		term_cursor += TERM_TAB_WIDTH - term_cursor % TERM_TAB_WIDTH;
		for(size_t i = old_term_cursor; i < term_cursor && i < VGA_WIDTH*VGA_HEIGHT; i++){
			term_buff[i] = vga_entry(' ', term_color);
		}
		term_scroll_if_needed(1, VGA_LAST_LINE);
		return;
	}

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
	term_clear();
	term_write_str("First line!\n");
	char buff[16];
	for(size_t i = 0; i < 23; i++){
		uitoa_hex(i, buff);
		term_write_str(buff);
		term_putchar('\n');
	}

	term_write_str("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
	term_write_str("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\n");
	term_write_str("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n");
	term_write_str("DDDD\n");

	term_cursor = term_cursor_pos(9, VGA_HEIGHT-3);
	term_putchar('\t');

}
