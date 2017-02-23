
#include <asm/io.h>

#include "pit.h"
#include "pic.h"

#define PIT_BASE 0x40
#define PIT_DATA_CH0 (PIT_BASE + 0)
#define PIT_DATA_CH1 (PIT_BASE + 1)
#define PIT_DATA_CH2 (PIT_BASE + 2)
#define PIT_CMD (PIT_BASE + 3)

void pit_cmd(uint8_t channel, uint8_t access_mode, uint8_t op_mode) {
	// Always use Binary Mode because BCD Mode is weird
	outb((channel & 0b11) << 6 | (access_mode & 0b11) << 4 | (op_mode & 0b111) << 1, PIT_CMD);
}

void pit_data_write16(uint8_t channel, uint16_t data) {
	outb(data & 0xff, PIT_BASE + channel);
	outb((data >> 8) & 0xff, PIT_BASE + channel);
}

void pit_data_write8(uint8_t channel, uint8_t data) {
	outb(data, PIT_BASE + channel);
}

uint16_t pit_data_read16(uint8_t channel) {
	uint16_t data;
	data = inb(PIT_BASE + channel);
	data |= inb(PIT_BASE + channel) << 8;
	return data;
}

uint8_t pit_data_read8(uint8_t channel) {
	return inb(PIT_BASE + channel);
}

void pit_init() {
	pic_unmask(1 << 0);
}

void pit_setup_interval(uint32_t frequency) {
	// Channel 0, Low+High byte, Rate Generator
	pit_cmd(0, 3, 2);
	// Set reload value
	pit_data_write16(0, 1193182 / frequency);
}
