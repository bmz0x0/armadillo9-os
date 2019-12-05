
#include <main.h>
#include <cpu.h>

#define ALIGN		.align 4,0x90
#define ENTRY(name) \
  .globl name; \
  ALIGN; \
  name:

#define PSR_F_BIT	0x00000040
#define PSR_I_BIT	0x00000080

#define SVC_MODE	0x00000013

#define RAM_BASE	0xC0000000
#define RAM_SIZE	0x2000000

#define PAGE_TABLE	0x14000
#define TEXT_OFFSET	0x180000
#define VIRT_OFFSET	-1073741824 /* PAGE_OFFSET = -0x40000000 */
#define MMUFLAG		0xC1E
#define TEXT_ADDR	0xC0018000

#define PERI_ADDR	0x80000000
#define PERI_SIZE	0x10000000

#define CONFIG_ALIGNMENT_TRAP	1

/*
 * CR1 bits (CP#15 CR1)
 */
#define CR_M	(1 << 0)	/* MMU enable				*/
#define CR_A	(1 << 1)	/* Alignment abort enable		*/
#define CR_C	(1 << 2)	/* Dcache enable			*/
#define CR_W	(1 << 3)	/* Write buffer enable			*/
#define CR_P	(1 << 4)	/* 32-bit exception handler		*/
#define CR_D	(1 << 5)	/* 32-bit data address range		*/
#define CR_L	(1 << 6)	/* Implementation defined		*/
#define CR_B	(1 << 7)	/* Big endian				*/
#define CR_S	(1 << 8)	/* System MMU protection		*/
#define CR_R	(1 << 9)	/* ROM MMU protection			*/
#define CR_F	(1 << 10)	/* Implementation defined		*/
#define CR_Z	(1 << 11)	/* Implementation defined		*/
#define CR_I	(1 << 12)	/* Icache enable			*/
#define CR_V	(1 << 13)	/* Vectors relocated to 0xffff0000	*/
#define CR_RR	(1 << 14)	/* Round Robin cache replacement	*/
#define CR_L4	(1 << 15)	/* LDR pc can set T bit			*/
#define CR_DT	(1 << 16)
#define CR_IT	(1 << 18)
#define CR_ST	(1 << 19)
#define CR_FI	(1 << 21)	/* Fast interrupt (lower latency mode)	*/
#define CR_U	(1 << 22)	/* Unaligned access operation		*/
#define CR_XP	(1 << 23)	/* Extended page tables			*/
#define CR_VE	(1 << 24)	/* Vectored interrupts			*/




	.section ".init.text","ax"
	.type	stext, %function
ENTRY(stext)
	msr		cpsr_c, #PSR_F_BIT | PSR_I_BIT | SVC_MODE
	bl		__create_page_tables

	adr		r13, __mmap_switched		@ address to jump to after
						@ mmu has been enabled
	adr		lr, __enable_mmu		@ return (PIC) address

//	adr		pc, initfunc
	adr		pc, __arm920_setup

	b		start_kernel


	.type	__create_page_tables, %function
__create_page_tables:
	mov		r4, #RAM_BASE
	add		r4, r4, #PAGE_TABLE

	/*
	 * Clear the 16K level 1 swapper page table
	 */
	mov	r0, r4
	mov	r3, #0
	add	r6, r0, #0x4000
1:	str	r3, [r0], #4
	str	r3, [r0], #4
	str	r3, [r0], #4
	str	r3, [r0], #4
	teq	r0, r6
	bne	1b

	ldr	r7, mmuflag

	mov	r6, pc, lsr #20			@ start of kernel section
	orr	r3, r7, r6, lsl #20		@ flags + kernel base
	str	r3, [r4, r6, lsl #2]		@ identity mapping

	add	r0, r4,  #(TEXT_ADDR & 0xff000000) >> 18	@ start of kernel
	str	r3, [r0, #(TEXT_ADDR & 0x00f00000) >> 18]!
	add	r3, r3, #1 << 20
	str	r3, [r0, #4]!			@ KERNEL + 1MB
	add	r3, r3, #1 << 20
	str	r3, [r0, #4]!			@ KERNEL + 2MB
	add	r3, r3, #1 << 20
	str	r3, [r0, #4]			@ KERNEL + 3MB

	bic	r7, r7, #0x0c			@ turn off cacheable
						@ and bufferable bits

	mov	r3, #PERI_ADDR
	mov	r6, r3, lsr #20			@ start of kernel section
	orr	r3, r7, r6, lsl #20		@ flags + kernel base

	add	r0, r4,  #(PERI_ADDR & 0xff000000) >> 18	@ start of kernel
	str	r3, [r0, #(PERI_ADDR & 0x00f00000) >> 18]!
1:
	add	r3, r3, #1 << 20
	cmp	r3, #(PERI_ADDR + PERI_SIZE)
	bhi	2f
	str	r3, [r0, #4]!
	b	1b
2:

#if 0
	add	r0, r4, #VIRT_OFFSET >> 18
	orr	r6, r5, r7
	str	r6, [r0]
#endif

	mov	pc, lr


	.type	__enable_mmu, %function
__enable_mmu:
#ifdef CONFIG_ALIGNMENT_TRAP
	orr	r0, r0, #CR_A
#else
	bic	r0, r0, #CR_A
#endif
#ifdef CONFIG_CPU_DCACHE_DISABLE
	bic	r0, r0, #CR_C
#endif
#ifdef CONFIG_CPU_BPREDICT_DISABLE
	bic	r0, r0, #CR_Z
#endif
#ifdef CONFIG_CPU_ICACHE_DISABLE
	bic	r0, r0, #CR_I
#endif
	mov	r5, #0x3F
	mcr	p15, 0, r5, c3, c0, 0		@ load domain access register
	mcr	p15, 0, r4, c2, c0, 0		@ load page table pointer

	b	__turn_mmu_on

	.align	5
	.type	__turn_mmu_on, %function
__turn_mmu_on:
	mov	r0, r0
	mcr	p15, 0, r0, c1, c0, 0		@ write control reg
	mrc	p15, 0, r3, c0, c0, 0		@ read id reg
	mov	r3, r3
	mov	r3, r3

	mov	pc, r13


	.type	__arm920_setup, #function
__arm920_setup:
	mov	r0, #0
	mcr	p15, 0, r0, c7, c7		@ invalidate I,D caches on v4
	mcr	p15, 0, r0, c7, c10, 4		@ drain write buffer on v4
	mcr	p15, 0, r0, c8, c7		@ invalidate I,D TLBs on v4
	mrc	p15, 0, r0, c1, c0		@ get control register v4
	ldr	r5, arm920_cr1_clear
	bic	r0, r0, r5
	ldr	r5, arm920_cr1_set
	orr	r0, r0, r5

	mov	pc, lr

	.type	arm920_cr1_clear, #object
	.type	arm920_cr1_set, #object
arm920_cr1_clear:
	.word	0x3f3f
arm920_cr1_set:
	.word	0x3135


	.type	__switch_data, %object
__switch_data:
	.long	__mmap_switched
	.long	__bss_start				@ r6
	.long	__bss_end				@ r7
	.long	kernel_stack + STACK_SIZE - 8	@ sp
@	.long	init_thread_union + THREAD_START_SP @ sp

	.type	__mmap_switched, %function
__mmap_switched:
	adr	r3, __switch_data + 4

	ldmia	r3, {r6, r7, sp}

	mov	fp, #0				@ Clear BSS (and zero fp)
1:	cmp	r6, r7
	strcc	fp, [r6],#4
	bcc	1b

	b	start_kernel

initfunc:
	.long		__arm920_setup

mmuflag:
	.long		MMUFLAG

