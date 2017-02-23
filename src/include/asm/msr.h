
#pragma once

static inline void msr_get(uint32_t msr, uint32_t *lo, uint32_t *hi) {
	asm volatile("rdmsr" : "=a" (*lo), "=d" (*hi) : "c" (msr));
}

static inline void msr_set(uint32_t msr, uint32_t lo, uint32_t hi) {
	asm volatile("wrmsr" : : "a" (lo), "d" (hi), "c" (msr));
}
