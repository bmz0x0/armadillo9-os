#ifndef _SETUP_H
#define _SETUP_H

#include "type.h"

#define TAG_PARAM_ADDR	((void *)0xC0000100)
#define PAGING_TABLE	((void *)0xC0014000)
#define MMUFLAG		0xC1E

#define NR_BANKS 8

struct meminfo {
	int nr_banks;
	struct {
		unsigned long start;
		unsigned long size;
		int           node;
	} bank[NR_BANKS];
};


/* The list ends with an ATAG_NONE node. */
#define ATAG_NONE	0x00000000

struct tag_header {
	u32 size;
	u32 tag;
};

/* The list must start with an ATAG_CORE node */
#define ATAG_CORE	0x54410001

struct tag_core {
	u32 flags;		/* bit 0 = read-only */
	u32 pagesize;
	u32 rootdev;
};

/* it is allowed to have multiple ATAG_MEM nodes */
#define ATAG_MEM	0x54410002

struct tag_mem32 {
	u32	size;
	u32	start;	/* physical start address */
};

/* board serial number. "64 bits should be enough for everybody" */
#define ATAG_SERIAL	0x54410006

struct tag_serialnr {
	u32 low;
	u32 high;
};

struct tag {
	struct tag_header hdr;
	union {
		struct tag_core		core;
		struct tag_mem32	mem;

		struct tag_serialnr	serialnr;
	} u;
};

struct tagtable {
	u32 tag;
	int (*parse)(const struct tag *);
};


/* Drivers should NOT use these either. */
#define __pa(x)			__virt_to_phys((unsigned long)(x))
#define __va(x)			((void *)__phys_to_virt((unsigned long)(x)))

/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT		12
#define PAGE_SIZE		(1UL << PAGE_SHIFT)
#define PAGE_MASK		(~(PAGE_SIZE-1))

/* to align the pointer to the (next) page boundary */
#define PAGE_ALIGN(addr)	(((addr)+PAGE_SIZE-1)&PAGE_MASK)

#define O_PFN_DOWN(x)	((x) >> PAGE_SHIFT)
#define V_PFN_DOWN(x)	O_PFN_DOWN(__pa(x))

#define O_PFN_UP(x)	(PAGE_ALIGN(x) >> PAGE_SHIFT)
#define V_PFN_UP(x)	O_PFN_UP(__pa(x))



#define __tag __attribute__((__used__)) __attribute__((__section__(".taglist")))
#define __tagtable(tag, fn) \
static struct tagtable __tagtable_##fn __tag = { tag, fn }

#define tag_next(t)	((struct tag *)((u32 *)(t) + (t)->hdr.size))
#define tag_size(type)	((sizeof(struct tag_header) + sizeof(struct type)) >> 2)


void setup_arch(void);

#endif // _SETUP_H


