
#include <linkage.h>
#include <asm.h>

	.text
/**
 * testop - implement a test_and_xxx_bit operation.
 * @instr: operational instruction
 * @store: store instruction
 *
 * Note: we can trivially conditionalise the store instruction
 * to avoid dirting the data cache.
 */
	.macro	bitop, instr
	and	r2, r0, #7
	mov	r3, #1
	mov	r3, r3, lsl r2
	save_and_disable_irqs ip, r2
	ldrb	r2, [r1, r0, lsr #3]
	\instr	r2, r2, r3
	strb	r2, [r1, r0, lsr #3]
	restore_irqs ip
	mov	pc, lr
	.endm


	.macro	testop, instr, store
	add	r1, r1, r0, lsr #3
	and	r3, r0, #7
	mov	r0, #1
	save_and_disable_irqs ip, r2
	ldrb	r2, [r1]
	tst	r2, r0, lsl r3
	\instr	r2, r2, r0, lsl r3
	\store	r2, [r1]
	restore_irqs ip
	moveq	r0, #0
	mov	pc, lr
	.endm

ENTRY(_test_and_clear_bit_le)
	testop	bicne, strneb

ENTRY(_test_and_set_bit_le)
	testop	orreq, streqb
