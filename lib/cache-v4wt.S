
#include <linkage.h>
#include <asm.h>

#define CACHE_DLINESIZE	32

	.text

ENTRY(flush_icache_range)
	bic	r0, r0, #CACHE_DLINESIZE - 1
1:	mcr	p15, 0, r0, c7, c5, 1		@ invalidate I entry
	add	r0, r0, #CACHE_DLINESIZE
	cmp	r0, r1
	blo	1b
	mov	pc, lr
