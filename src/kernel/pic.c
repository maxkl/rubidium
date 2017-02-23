
#include <asm/io.h>

#include "pic.h"

#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xa0
#define PIC2_DATA 0xa1

#define ICW1_ICW4 0x01			// ICW4 (not) needed
#define ICW1_SINGLE 0x02		// Single (cascade) mode
#define ICW1_INTERVAL4 0x04		// Call address interval 4 (8)
#define ICW1_LEVEL 0x08			// Level triggered (edge) mode
#define ICW1_INIT 0x10			// Initialization - required!

#define ICW4_8086 0x01			// 8086/88 (MCS-80/85) mode
#define ICW4_AUTO 0x02			// Auto (normal) EOI
#define ICW4_BUF_SLAVE 0x08		// Buffered mode/slave
#define ICW4_BUF_MASTER 0x0C	// Buffered mode/master
#define ICW4_SFNM 0x10			// Special fully nested (not)

#define PIC_READ_IRR 0x0a
#define PIC_READ_ISR 0x0b

#define PIC_EOI 0x20

uint16_t pic_get_reg(uint8_t cmd) {
	outb(PIC1_CMD, cmd);
	outb(PIC2_CMD, cmd);
	return (inb(PIC2_CMD) << 8) | inb(PIC1_CMD);
}

uint16_t pic_get_irr() {
	return pic_get_reg(PIC_READ_IRR);
}

uint16_t pic_get_isr() {
	return pic_get_reg(PIC_READ_ISR);
}

void pic_disable() {
	// Mask all interrupts
	outb(0xff, PIC1_DATA);
	outb(0xff, PIC2_DATA);
}

bool pic_is_spurious_irq(uint8_t irq) {
	if(irq != 7 && irq != 15) {
		return false;
	}

	uint16_t isr = pic_get_isr();
	if(isr & (1 << irq)) {
		return false;
	}

	return true;
}

void pic_eoi(uint8_t irq) {
	if(irq >= 8) {
		outb(PIC_EOI, PIC2_CMD);
	}
	outb(PIC_EOI, PIC1_CMD);
}

void pic_eoi_spurious(uint8_t irq) {
	if(irq >= 8) {
		outb(PIC_EOI, PIC1_CMD);
	}
}

void pic_set_mask(uint16_t mask) {
	outb(mask & 0xff, PIC1_DATA);
	outb(mask >> 8, PIC2_DATA);
}

void pic_mask(uint16_t irqs) {
	outb(inb(PIC1_DATA) | (irqs & 0xff), PIC1_DATA);
	outb(inb(PIC2_DATA) | (irqs >> 8), PIC2_DATA);
}

void pic_unmask(uint16_t irqs) {
	outb(inb(PIC1_DATA) & ~(irqs & 0xff), PIC1_DATA);
	outb(inb(PIC2_DATA) & ~(irqs >> 8), PIC2_DATA);
}

void pic_init() {
	// Initialize
	outb(ICW1_INIT | ICW1_ICW4, PIC1_CMD);
	outb(ICW1_INIT | ICW1_ICW4, PIC2_CMD);
	// Interrupt vector offset
	outb(0x20, PIC1_DATA);
	outb(0x28, PIC2_DATA);
	//
	outb(4, PIC1_DATA);
	outb(2, PIC2_DATA);
	// Set 80x86 mode
	outb(ICW4_8086, PIC1_DATA);
	outb(ICW4_8086, PIC2_DATA);

	// Mask all interrupts
	pic_set_mask(0xffff);
}
