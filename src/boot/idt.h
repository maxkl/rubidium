
#pragma once

#include <stdint.h>

#define INT_COUNT 256

struct idt_desc {
	uint16_t offset_low;
	uint16_t segment;
	uint8_t zero;
	unsigned type : 5, dpl : 2, p : 1;
	uint16_t offset_high;
} __attribute__((packed));

struct idt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

struct idt_state {
	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;

	uint32_t int_num;
	uint32_t err_code;

	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t ss;
} __attribute__((packed));

extern struct idt_desc idt[INT_COUNT];

static inline void load_idt(const struct idt_ptr *ptr) {
	asm volatile("lidt %0" : : "m" (*ptr));
}

static inline void sti() {
	asm volatile("sti");
}

static inline void cli() {
	asm volatile("cli");
}

void idt_init();
