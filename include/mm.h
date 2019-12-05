#ifndef _MM_H
#define _MM_H

#include "type.h"

//extern void * memset(void *, int, __kernel_size_t);

extern void __memzero(void *ptr, __kernel_size_t n);

#define memzero(p,n) 							\
	({ 								\
	 	void *__p = (p); size_t __n = n;			\
	 	if ((__n) != 0) 					\
	 		__memzero((__p),(__n)); 			\
	 	(__p); 							\
	 })

static inline int __test_bit(int nr, const volatile unsigned long * p)
{
	return (p[nr >> 5] >> (nr & 31)) & 1UL;
}


// bitops.h
extern int _test_and_set_bit_le(int nr, volatile unsigned long * p);
extern int _test_and_clear_bit_le(int nr, volatile unsigned long * p);
extern int _find_first_zero_bit_le(const void * p, unsigned size);
extern int _find_next_zero_bit_le(const void * p, int size, int offset);
extern int _find_first_bit_le(const unsigned long *p, unsigned size);
extern int _find_next_bit_le(const unsigned long *p, int size, int offset);

#if 1 // FIXME
#define	ATOMIC_BITOP_LE(name,nr,p)		_##name##_le(nr, p)
#else
#define	ATOMIC_BITOP_LE(name,nr,p)		\
	(__builtin_constant_p(nr) ?		\
	 ____atomic_##name(nr, p) :		\
	 _##name##_le(nr,p))
#endif

#define test_and_set_bit(nr,p)		ATOMIC_BITOP_LE(test_and_set_bit,nr,p)
#define test_and_clear_bit(nr,p)		ATOMIC_BITOP_LE(test_and_clear_bit,nr,p)
#define test_bit(nr,p)			__test_bit(nr,p)
#define find_first_zero_bit(p,sz)	_find_first_zero_bit_le(p,sz)
#define find_next_zero_bit(p,sz,off)	_find_next_zero_bit_le(p,sz,off)
#define find_first_bit(p,sz)		_find_first_bit_le(p,sz)
#define find_next_bit(p,sz,off)		_find_next_bit_le(p,sz,off)


// page table
#define PAGING_TABLE	((void *)0xC0014000)

#define ENT_PER_L1PT		4096
#define ENT_PER_L2PT		256

typedef unsigned long l1pte_t;
typedef unsigned long l2pte_t;

#define l1pte_off(addr)		(((unsigned long *)PAGING_TABLE) + ((addr) >> 20))


typedef struct bootmem_data {
	unsigned long node_boot_start;
	unsigned long node_low_pfn;
	void *node_bootmem_map;
	unsigned long last_offset;
	unsigned long last_pos;
	unsigned long last_success;	/* Previous allocation point.  To speed
					 * up searching */
} bootmem_data_t;


#define ALIGN(x,a) (((x)+(a)-1)&~((a)-1))

// cache.S
extern void flush_icache_range(unsigned long, unsigned long);

#endif // _MM_H

