
#include <vector.h>
#include <cpu.h>

#define SYS_ERROR0 10420224 /* 0x9f0000 */

	.macro  get_irqnr_and_base, irqnr, irqstat, base, tmp
	ldr	\irqstat, =VIC0IRQSTATUS
	ldr	\irqstat, [\irqstat, #0]

		mov	\irqnr, #0

1001:	tst	\irqstat, #1
	bne	1003f
	add	\irqnr, \irqnr, #1
	mov	\irqstat, \irqstat, lsr #1
	cmp	\irqnr, #32
	bcc	1001b

	ldr	\irqstat, =VIC1IRQSTATUS
	ldr	\irqstat, [\irqstat, #0]

1002:	tst	\irqstat, #1
	bne	1003f
	add	\irqnr, \irqnr, #1
	mov	\irqstat, \irqstat, lsr #1
	cmp	\irqnr, #64
	bcc	1002b
1003:
	.endm


	.macro	svc_entry, sym
	sub	sp, sp, #S_FRAME_SIZE
	stmia	sp, {r0 - r12}			@ save r0 - r12
	ldr	r2, .LC\sym
	add	r0, sp, #S_FRAME_SIZE
	ldmia	r2, {r2 - r4}			@ get pc, cpsr
	add	r5, sp, #S_SP
	mov	r1, lr

	@
	@ We are now ready to fill in the remaining blanks on the stack:
	@
	@  r0 - sp_svc
	@  r1 - lr_svc
	@  r2 - lr_<exception>, already fixed up for correct return/restart
	@  r3 - spsr_<exception>
	@  r4 - orig_r0 (see pt_regs definition in ptrace.h)
	@
	stmia	r5, {r0 - r4}
	.endm


	.macro	vector_stub, name, sym, correction=0
	.align	5

vector_\name:
	ldr	r13, .LCs\sym
	.if \correction
	sub	lr, lr, #\correction
	.endif
	str	lr, [r13]			@ save lr_IRQ
	mrs	lr, spsr
	str	lr, [r13, #4]			@ save spsr_IRQ
	@
	@ now branch to the relevant MODE handling routine
	@
	mrs	r13, cpsr
	bic	r13, r13, #MODE_MASK
	orr	r13, r13, #SVC_MODE
	msr	spsr_cxsf, r13			@ switch to SVC_32 mode

	and	lr, lr, #15
	ldr	lr, [pc, lr, lsl #2]
	movs	pc, lr				@ Changes mode and branches
	.endm


	.section	".init.text","ax"

	.align	5
__irq_svc:
	sub	sp, sp, #S_FRAME_SIZE
	stmia	sp, {r0 - r12}			@ save r0 - r12
	ldr	r2, .LCsirq
	add	r0, sp, #S_FRAME_SIZE
	ldmia	r2, {r2 - r4}			@ get pc, cpsr
	add	r5, sp, #S_SP
	mov	r1, lr

	@
	@ We are now ready to fill in the remaining blanks on the stack:
	@
	@  r0 - sp_svc
	@  r1 - lr_svc
	@  r2 - lr_<exception>, already fixed up for correct return/restart
	@  r3 - spsr_<exception>
	@  r4 - orig_r0 (see pt_regs definition in ptrace.h)
	@
	stmia	r5, {r0 - r4}

1:	get_irqnr_and_base r0, r6, r5, lr
	movne	r1, sp
	@
	@ routine called with r0 = irq number, r1 = struct pt_regs *
	@
	adrne	lr, 1b

	bne	asm_do_IRQ
	ldr	r0, [sp, #S_PSR]		@ irqs are already disabled
	msr	spsr_cxsf, r0
	ldmia	sp, {r0 - pc}^			@ load r0 - pc, cpsr

	.align	5

	.globl	__vectors_start
__vectors_start:
	swi	SYS_ERROR0
	b	vector_und
	b	vector_swi
	b	vector_pabt
	b	vector_dabt
	b	vector_addrexcptn
	b	vector_irq
	b	vector_fiq

vector_und:
vector_swi:
vector_pabt:
vector_dabt:
vector_addrexcptn:
vector_fiq:
	b	vector_und

vector_irq:
	ldr	r13, .LCsirq
	sub	lr, lr, #4

	str	lr, [r13]			@ save lr_IRQ
	mrs	lr, spsr
	str	lr, [r13, #4]			@ save spsr_IRQ
	@
	@ now branch to the relevant MODE handling routine
	@
	mrs	r13, cpsr
	bic	r13, r13, #MODE_MASK
	orr	r13, r13, #SVC_MODE
	msr	spsr_cxsf, r13			@ switch to SVC_32 mode
	mov	r0, r0
	mov	r0, r0
	ldr	lr, .LCirq_svc
	movs	pc, lr

.LCirq_svc:
	.long	__irq_svc

.LCsirq:
	.word	__temp_irq

	.globl	__vectors_end

__vectors_end:

	.data


__temp_irq:
	.word	0				@ saved lr_irq
	.word	0				@ saved spsr_irq
	.word	-1				@ old_r0


