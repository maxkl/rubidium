
#pragma once

#include <stdint.h>
#include <stdbool.h>

void pic_init();
void pic_set_mask(uint16_t irqs);
void pic_mask(uint16_t irqs);
void pic_unmask(uint16_t irqs);
void pic_disable();
bool pic_is_spurious_irq(uint8_t irq);
void pic_eoi(uint8_t irq);
void pic_eoi_spurious(uint8_t irq);
