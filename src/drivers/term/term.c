
#include <string.h>

#include <drivers/term.h>
#include <asm/io.h>

#define vga_entry_color(fg, bg) ((uint8_t) (fg) | (uint8_t) (bg) << 4)
#define vga_entry(c, color) ((uint16_t) (c) | (uint16_t) (color) << 8)

#define VGA_MEMORY_ADDRESS 0xb8000
#define VGA_SCREEN_CTRL 0x3d4
#define VGA_SCREEN_DATA 0x3d5

uint8_t *vga_memory;
uint8_t term_row, term_col, vga_width, vga_height;
uint8_t term_color;

void term_init() {
	vga_memory = (uint8_t *) VGA_MEMORY_ADDRESS;
	term_row = 0;
	term_col = 0;
	vga_width = 80;
	vga_height = 25;

	term_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

	size_t n = vga_width * vga_height * 2;
	for(size_t i = 0; i < n; i += 2) {
		vga_memory[i] = ' ';
		vga_memory[i + 1] = term_color;
	}
}

void term_set_color(enum vga_color fg, enum vga_color bg) {
	term_color = vga_entry_color(fg, bg);
}

void set_cursor(uint16_t offset) {
	outb(0xe, VGA_SCREEN_CTRL);
	outb(offset >> 8, VGA_SCREEN_DATA);
	outb(0xf, VGA_SCREEN_CTRL);
	outb(offset & 0xff, VGA_SCREEN_DATA);
}

void term_scroll(uint8_t lines) {
	uint8_t *mem_dest = vga_memory;
	uint8_t *mem_src = &vga_memory[lines * vga_width * 2];
	size_t n = (vga_height - lines) * vga_width * 2;
	memmove(mem_dest, mem_src, n);

	size_t start = vga_width * (vga_height - 1) * 2;
	for(size_t i = 0; i < vga_width * 2; i += 2) {
		vga_memory[start + i] = ' ';
		vga_memory[start + i + 1] = term_color;
	}

	term_row -= lines;
}

void term_putc(char c) {
	if(c == '\n') {
		term_col = 0;
		term_row++;
		if(term_row >= vga_height) {
			term_scroll(1);
		}
	} else {
		size_t addr = (term_row * vga_width + term_col) * 2;
		vga_memory[addr] = c;
		vga_memory[addr + 1] = term_color;

		term_col++;
		if(term_col >= vga_width) {
			term_col = 0;
			term_row++;
			if(term_row >= vga_height) {
				term_scroll(1);
			}
		}
	}
	set_cursor(term_row * vga_width + term_col);
}

void term_puts(const char *str) {
	while (*str != '\0') {
		term_putc(*str);
		str++;
	}
}

void term_printi(int v, unsigned int r) {
	if(r < 2 || r > 36)
		return;

	unsigned int uv;
	if(v < 0 && r == 10) {
		uv = -v;
		term_putc('-');
	} else {
		uv = v;
	}

	unsigned int divider = 1;
	unsigned int tmp = uv;
	do {
		tmp /= r;
		if(tmp)
			divider *= r;
	} while(tmp);

	while(divider > 0) {
		unsigned int c = (uv / divider) % r;
		c += c > 9 ? ('A' - 10) : '0';
		term_putc(c);
		divider /= r;
	}
}
