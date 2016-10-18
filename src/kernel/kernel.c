
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <drivers/term.h>

void kernel_main() {
	term_init();
	term_puts("Starting kernel...\n");
	term_puts("It's so ");
	term_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	term_putc('b');
	term_set_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
	term_putc('e');
	term_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
	term_putc('a');
	term_set_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
	term_putc('u');
	term_set_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	term_putc('t');
	term_set_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
	term_putc('i');
	term_set_color(VGA_COLOR_MAGENTA, VGA_COLOR_BLACK);
	term_putc('f');
	term_set_color(VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
	term_putc('u');
	term_set_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
	term_putc('l');
	term_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
	term_puts("!\n");
}
