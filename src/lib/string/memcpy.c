
#include <string.h>

void *memcpy(void * restrict dest, const void * restrict src, size_t n) {
	char *dp = dest;
	const char *sp = src;
	while(n--)
		*dp++ = *sp++;
	return dest;
}
