
#include <stdint.h>

#include <drivers/term.h>

// TODO: randomize
uintptr_t __stack_chk_guard = 0xCD000AFFUL;

__attribute__((noreturn))
void __stack_chk_fail() {
	term_puts("Error: Stack smashed");
	while(1)
		asm volatile("hlt");
}
