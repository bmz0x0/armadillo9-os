
#include <linkage.h>
#include <asm.h>

		.text
/*
 * Purpose  : Find a 'zero' bit
 * Prototype: int find_first_zero_bit(void *addr, unsigned int maxbit);
 */
ENTRY(_find_first_zero_bit_le)
		teq	r1, #0	
		beq	3f
		mov	r2, #0
1:		ldrb	r3, [r0, r2, lsr #3]
		eors	r3, r3, #0xff		@ invert bits
		bne	.found			@ any now set - found zero bit
		add	r2, r2, #8		@ next bit pointer
2:		cmp	r2, r1			@ any more?
		blo	1b
3:		mov	r0, r1			@ no free bits
		RETINSTR(mov,pc,lr)


ENTRY(_find_next_zero_bit_le)
		teq	r1, #0
		beq	3b
		ands	ip, r2, #7
		beq	1b			@ If new byte, goto old routine
		ldrb	r3, [r0, r2, lsr #3]
		eor	r3, r3, #0xff		@ now looking for a 1 bit
		movs	r3, r3, lsr ip		@ shift off unused bits
		bne	.found
		orr	r2, r2, #7		@ if zero, then no bits here
		add	r2, r2, #1		@ align bit pointer
		b	2b			@ loop for next bit

/*
 * Purpose  : Find a 'one' bit
 * Prototype: int find_first_bit(const unsigned long *addr, unsigned int maxbit);
 */
ENTRY(_find_first_bit_le)
		teq	r1, #0
		beq	3f
		mov	r2, #0
1:		ldrb	r3, [r0, r2, lsr #3]
		movs	r3, r3
		bne	.found			@ any now set - found zero bit
		add	r2, r2, #8		@ next bit pointer
2:		cmp	r2, r1			@ any more?
		blo	1b
3:		mov	r0, r1			@ no free bits
		RETINSTR(mov,pc,lr)

/*
 * Purpose  : Find next 'one' bit
 * Prototype: int find_next_zero_bit(void *addr, unsigned int maxbit, int offset)
 */
ENTRY(_find_next_bit_le)
		teq	r1, #0
		beq	3b
		ands	ip, r2, #7
		beq	1b			@ If new byte, goto old routine
		ldrb	r3, [r0, r2, lsr #3]
		movs	r3, r3, lsr ip		@ shift off unused bits
		bne	.found
		orr	r2, r2, #7		@ if zero, then no bits here
		add	r2, r2, #1		@ align bit pointer
		b	2b			@ loop for next bit


/*
 * One or more bits in the LSB of r3 are assumed to be set.
 */
.found:
#if __LINUX_ARM_ARCH__ >= 5
		rsb	r1, r3, #0
		and	r3, r3, r1
		clz	r3, r3
		rsb	r3, r3, #31
		add	r0, r2, r3
#else
		tst	r3, #0x0f
		addeq	r2, r2, #4
		movne	r3, r3, lsl #4
		tst	r3, #0x30
		addeq	r2, r2, #2
		movne	r3, r3, lsl #2
		tst	r3, #0x40
		addeq	r2, r2, #1
		mov	r0, r2
#endif
		RETINSTR(mov,pc,lr)

