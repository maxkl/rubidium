
#include <string.h>

#include <drivers/term.h>

#include "idt.h"
#include "../kernel/pic.h"

struct idt_desc idt[INT_COUNT];

void irq_handler(struct idt_state *state) {
	uint8_t irq = state->int_num - 32;
	if(pic_is_spurious_irq(irq)) {
		term_puts("Spurious IRQ");
		term_printi(irq, 10);
		term_puts("\n");

		pic_eoi_spurious(irq);
	} else {
		term_puts("IRQ");
		term_printi(irq, 10);
		term_puts("\n");

		pic_eoi(irq);
	}
}

void idt_handler(struct idt_state *state) {
	if(state->int_num >= 32) {
		irq_handler(state);
	} else {
		term_puts("Interrupt 0x");
		term_printi(state->int_num, 16);
		term_puts(" (0x");
		term_printi(state->err_code, 16);
		term_puts(")\n");
	}
}

void set_idt_entry(uint8_t num, const void *handler) {
	idt[num].offset_low = (uint32_t) handler & 0xffff;
	idt[num].offset_high = ((uint32_t) handler >> 16) & 0xffff;
	idt[num].segment = 0x08;
	idt[num].zero = 0;
	idt[num].type = 0b01110;
	idt[num].dpl = 0;
	idt[num].p = 1;
}

extern const char idt_handlers[32][9];
extern const char irq_handlers[16][9];

void idt_init() {
	// In BSS so it should be all zeroes already
	// memset(idt, 0, sizeof(idt));

	for(unsigned int i = 0; i < 32; i++) {
		set_idt_entry(i, idt_handlers[i]);
	}

	for(unsigned int i = 0; i < 16; i++) {
		set_idt_entry(32 + i, irq_handlers[i]);
	}

	struct idt_ptr ptr;
	ptr.limit = sizeof(idt) - 1;
	ptr.base = (uint32_t) idt;
	load_idt(&ptr);
}
