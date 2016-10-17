
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

#define vga_entry_color(fg, bg) (fg | bg << 4)
#define vga_entry(c, color) ((uint16_t) c | (uint16_t) color << 8)

void kernel_main() {
	uint16_t *vga_mem = (uint16_t *) 0xb8000;
	*vga_mem = vga_entry('X', vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_WHITE));
}
