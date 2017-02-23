
#pragma once

static inline void cpuid(uint32_t code, uint32_t *a, uint32_t *d) {
	asm volatile("cpuid" : "=a" (*a), "=d" (*d) : "a" (code) : "ecx", "ebx");
}
