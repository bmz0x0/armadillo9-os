
#include <type.h>
#include <setup.h>
#include <sci.h>
#include <memory.h>
#include <init.h>
#include <mm.h>
#include <printk.h>

extern void __start, __end;

static struct meminfo meminfo __initdata = { 0, };

extern void bootmem_init(struct meminfo *);
extern void * __alloc_bootmem(unsigned long size, unsigned long align, unsigned long goal);
extern void alloc_init_page(unsigned long virt, unsigned long phys, unsigned long prot_l1, unsigned long prot);

void * __init memcpy(void * dest,const void *src,unsigned int count)
{
	char *tmp = (char *) dest, *s = (char *) src;

	printf("func memcpy dest = %lX, src = %lX, count = %lX\n"
		, (unsigned long)dest, (unsigned long)src, (unsigned long)count);

	while (count--)
		*tmp++ = *s++;

	return dest;
}

static int __init parse_tag_mem32(const struct tag *tag)
{
	sci_write("parse_tag_mem32\n");

	if (meminfo.nr_banks >= NR_BANKS) {
		printf("parse_tag_mem32 error.\n");
		return -1;
	}
	meminfo.bank[meminfo.nr_banks].start = tag->u.mem.start;
	meminfo.bank[meminfo.nr_banks].size  = tag->u.mem.size;
	meminfo.nr_banks += 1;

	return 0;
}
__tagtable(ATAG_MEM, parse_tag_mem32);

void __init parse_tags(const struct tag *tag)
{
	extern struct tagtable __tagtable_begin, __tagtable_end;
	struct tagtable *t;

	for (; tag->hdr.size; tag = tag_next(tag)) {
		for (t = &__tagtable_begin; t < &__tagtable_end; t++) {
			if (tag->hdr.tag == t->tag) {
				t->parse(tag);
				break;
			}
		}

		if (t >= &__tagtable_end) {
			printf("unknown tag.\n");
		}
	}
}

void __init set_section(unsigned long virt, unsigned long phys, unsigned long flag)
{
	unsigned long *p;

	p = l1pte_off(virt);

	// FIXME: please settings domain.
	*p = (u32)(( (unsigned long)phys & 0xFFF00000 ) + flag);

	asm("mcr	p15, 0, %0, c7, c10, 1" : : "r"(p));
	asm("mcr	p15, 0, %0, c7, c10, 4" : : "r" (0));
}

void __init paging_init(struct meminfo *mi)
{
	extern char __vectors_start[], __vectors_end[];
	unsigned long *ptr;
	unsigned long i, j;
	unsigned long cpu_id;

	// bootmem_init
	bootmem_init(mi);

	printf("set_section\n");

	for ( i = 0; i < mi->nr_banks; i++ ) {
		if (mi->bank[i].size == 0)
			continue;

		// FIXME: (1<<20) == PAGE_SIZE
		for ( j = 0; j < mi->bank[i].size; j+=(1<<20) ) {
			set_section( (unsigned long)(phys_to_virt(mi->bank[i].start) + j), (mi->bank[i].start + j), MMUFLAG );
		}
	}

	ptr = __alloc_bootmem(PAGE_SIZE, PAGE_SIZE, 0);
	printf("alloc_bootmem = %lX\n", ptr);

	printf("set_section2\n");

	// FIXME
	alloc_init_page((unsigned long)0xFFFF0000, (unsigned long)ptr, 0x09, 0xFFE);

	// flush_cache
	asm("mcr p15, 0, %0, c7, c10, 4" : : "r" (0));
	asm("mcr p15, 0, %0, c7, c5, 0" : : "r"(0));
	//asm("mcr p15, 0, %0, c7, c6, 0" : : "r"(0));

	// flush_tlb_all();
	asm("mcr p15, 0, %0, c8, c7, 0" : : "r"(0));

	// cpu_id
	asm("mcr p15, 0, %0, c0, c0" : "=r"(cpu_id) : );
	printf("cpu_id = %lX\n", cpu_id);

	printf("memcpy\n");

	memcpy((void *)0xFFFF0000, __vectors_start, __vectors_end - __vectors_start);
	flush_icache_range(0xffff0000, 0xffff0000 + PAGE_SIZE);
}

void __init setup_arch()
{
	struct tag *tags = TAG_PARAM_ADDR;

	if (tags->hdr.tag == ATAG_CORE ) {
		parse_tags(tags);
	}

	paging_init(&meminfo);
}

