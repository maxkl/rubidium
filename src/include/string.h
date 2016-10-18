
#pragma once

#include <stddef.h>

int memcmp(const void *, const void *, size_t);
void *memset(void *, int, size_t);
void *memcpy(void * restrict, const void * restrict, size_t);
void *memmove(void *, const void *, size_t);
