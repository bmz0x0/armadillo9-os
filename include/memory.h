#ifndef _MEMORY_H
#define _MEMORY_H

/*
 * Page offset: 3GB
 */
#ifndef PAGE_OFFSET
#define PAGE_OFFSET		(0xc0000000UL)
#endif

#ifndef PHYS_OFFSET
#define PHYS_OFFSET		(0xc0000000UL)
#endif

#define __virt_to_phys(x)	((x) - PAGE_OFFSET + PHYS_OFFSET)
#define __phys_to_virt(x)	((x) - PHYS_OFFSET + PAGE_OFFSET)


static inline unsigned long virt_to_phys(void *x)
{
	return __virt_to_phys((unsigned long)(x));
}

static inline void *phys_to_virt(unsigned long x)
{
	return (void *)(__phys_to_virt((unsigned long)(x)));
}


#endif // _MEMORY_H

