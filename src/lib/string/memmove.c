
#include <string.h>

void *memmove(void *dest, const void *src, size_t n) {
	char *dp = dest;
	const char *sp = src;
	if(sp > dp) {
		// Copy forwards
		while(n--)
			*dp++ = *sp++;
	} else if(sp < dp) {
		// Copy backwards
		dp += n;
		sp += n;
		while(n--)
			*--dp = *--sp;
	}
	// If sp == dp, nothing has to be done

	return dest;
}
