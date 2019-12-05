
#include <main.h>
#include <mm.h>
#include <memory.h>
#include <setup.h>
#include <sci.h>
#include <printk.h>

extern void __start, __end;

bootmem_data_t bpgdata;


void * memset(void * s,int c,size_t count)
{
	char *xs = (char *) s;

	while (count--)
		*xs++ = c;

	return s;
}

static unsigned long __init init_bootmem_core (unsigned long mapstart, unsigned long start, unsigned long end)
{
	bootmem_data_t *bdata = &bpgdata;
	unsigned long mapsize = ((end - start)+7)/8;

	mapsize = (mapsize + (sizeof(long) - 1UL)) & ~(sizeof(long) - 1UL);
	bdata->node_bootmem_map = phys_to_virt(mapstart << PAGE_SHIFT);
	bdata->node_boot_start = (start << PAGE_SHIFT);
	bdata->node_low_pfn = end;

	/*
	 * Initially all pages are reserved - setup_arch() has to
	 * register free RAM areas explicitly.
	 */
	printf("init_bootmem_core memset!.\n");
	printf("node_bootmem_map = %lX\n", (unsigned long)bdata->node_bootmem_map);
	printf("mapsize = %lX\n", mapsize);

	memset(bdata->node_bootmem_map, 0xff, mapsize);

	return mapsize;
}

static void * __init __alloc_bootmem_core(struct bootmem_data *bdata, unsigned long size,
											unsigned long align, unsigned long goal)
{
	unsigned long offset, remaining_size, areasize, preferred;
	unsigned long i, start = 0, incr, eidx;
	void *ret;

	if(!size) {
		printf("__alloc_bootmem_core(): zero-sized request\n");
		return NULL;
	}

	eidx = bdata->node_low_pfn - (bdata->node_boot_start >> PAGE_SHIFT);

	offset = 0;
	if (align && (bdata->node_boot_start & (align - 1UL)) != 0)
		offset = (align - (bdata->node_boot_start & (align - 1UL)));
	offset >>= PAGE_SHIFT;

	/*
	 * We try to allocate bootmem pages above 'goal'
	 * first, then we try to allocate lower pages.
	 */
	if (goal && (goal >= bdata->node_boot_start) && 
	    ((goal >> PAGE_SHIFT) < bdata->node_low_pfn)) {
		preferred = goal - bdata->node_boot_start;

		if (bdata->last_success >= preferred)
			preferred = bdata->last_success;
	} else
		preferred = 0;

	preferred = ((preferred + align - 1) & ~(align - 1)) >> PAGE_SHIFT;
	preferred += offset;
	areasize = (size+PAGE_SIZE-1)/PAGE_SIZE;
	incr = align >> PAGE_SHIFT ? : 1;

restart_scan:
	for (i = preferred; i < eidx; i += incr) {
		unsigned long j;
		i = find_next_zero_bit(bdata->node_bootmem_map, eidx, i);
		i = ALIGN(i, incr);

		if (test_bit(i, bdata->node_bootmem_map)) {
			continue;
		}
		for (j = i + 1; j < i + areasize; ++j) {
			if (j >= eidx)
				goto fail_block;
			if (test_bit (j, bdata->node_bootmem_map))
				goto fail_block;
		}
		start = i;
		goto found;
	fail_block:
		i = ALIGN(j, incr);
	}

	if (preferred > offset) {
		preferred = offset;
		goto restart_scan;
	}
	return NULL;

found:
	bdata->last_success = start << PAGE_SHIFT;
#if 0 // FIXME
	BUG_ON(start >= eidx);
#endif
	printf("__alloc_bootmem_core() : found free bootmem.\n");

	/*
	 * Is the next page of the previous allocation-end the start
	 * of this allocation's buffer? If yes then we can 'merge'
	 * the previous partial page with this allocation.
	 */
	if (align < PAGE_SIZE &&
	    bdata->last_offset && bdata->last_pos+1 == start) {
		offset = (bdata->last_offset+align-1) & ~(align-1);
#if 0 // FIXME
		BUG_ON(offset > PAGE_SIZE);
#endif
		remaining_size = PAGE_SIZE-offset;
		if (size < remaining_size) {
			areasize = 0;
			/* last_pos unchanged */
			bdata->last_offset = offset+size;
			ret = phys_to_virt(bdata->last_pos*PAGE_SIZE + offset +
						bdata->node_boot_start);
		} else {
			remaining_size = size - remaining_size;
			areasize = (remaining_size+PAGE_SIZE-1)/PAGE_SIZE;
			ret = phys_to_virt(bdata->last_pos*PAGE_SIZE + offset +
						bdata->node_boot_start);
			bdata->last_pos = start+areasize-1;
			bdata->last_offset = remaining_size;
		}
		bdata->last_offset &= ~PAGE_MASK;
	} else {
		bdata->last_pos = start + areasize - 1;
		bdata->last_offset = size & ~PAGE_MASK;
		ret = phys_to_virt(start * PAGE_SIZE + bdata->node_boot_start);
	}

	/*
	 * Reserve the area now:
	 */
	for (i = start; i < start+areasize; i++) {
		if (test_and_set_bit(i, bdata->node_bootmem_map)) {
#if 1 // FIXME
			printf("Error __alloc_bootmem_core() : test_and_set_bit.\n");
#else
			BUG();
#endif
		}
	}
	memset(ret, 0, size);
	return ret;
}

void * __init __alloc_bootmem (unsigned long size, unsigned long align, unsigned long goal)
{
	bootmem_data_t *bdata = &bpgdata;
	void *ptr;

	ptr = __alloc_bootmem_core(bdata, size, align, goal);

	return ptr;
}

static void __init free_bootmem_core(bootmem_data_t *bdata, unsigned long addr, unsigned long size)
{
	unsigned long i;
	unsigned long start;
	/*
	 * round down end of usable mem, partially free pages are
	 * considered reserved.
	 */
	unsigned long sidx;
	unsigned long eidx = (addr + size - bdata->node_boot_start)/PAGE_SIZE;
	//unsigned long end = (addr + size)/PAGE_SIZE;

#if 0
	BUG_ON(!size);
	BUG_ON(end > bdata->node_low_pfn);
#endif

	if (addr < bdata->last_success)
		bdata->last_success = addr;

	/*
	 * Round up the beginning of the address.
	 */
	start = (addr + PAGE_SIZE-1) / PAGE_SIZE;
	sidx = start - (bdata->node_boot_start/PAGE_SIZE);

	for (i = sidx; i < eidx; i++) {
		// FIXME : refer to bootmem.c
		test_and_clear_bit(i, bdata->node_bootmem_map);
	}

	printf("free_bootmem(): bpgdata->node_bootmem_map = %lX\n", bpgdata.node_bootmem_map);
	printf("free_bootmem(): sidx = %lX, eidx = %lX\n", sidx, eidx);

}

static inline void free_bootmem_bank(struct meminfo *mi)
{
	bootmem_data_t *bdata = &bpgdata;
	int bank;

	for (bank = 0; bank < mi->nr_banks; bank++) {
		free_bootmem_core(bdata, mi->bank[bank].start, mi->bank[bank].size);
	}
}

static void __init reserve_bootmem_core(bootmem_data_t *bdata, unsigned long addr, unsigned long size)
{
	unsigned long i;
	/*
	 * round up, partially reserved pages are considered
	 * fully reserved.
	 */
	unsigned long sidx = (addr - bdata->node_boot_start)/PAGE_SIZE;
	unsigned long eidx = (addr + size - bdata->node_boot_start + 
							PAGE_SIZE-1)/PAGE_SIZE;
#if 0 // FIXME
	unsigned long end = (addr + size + PAGE_SIZE-1)/PAGE_SIZE;
#endif

	for (i = sidx; i < eidx; i++)
		if (test_and_set_bit(i, bdata->node_bootmem_map)) {
			// debug message
		}
}

/*
 * Reserve the various regions of node 0
 */
static __init void reserve_node_zero(unsigned int bootmap_pfn, unsigned int bootmap_pages)
{
	bootmem_data_t *bdata = &bpgdata;
	unsigned long res_size = 0;

	/*
	 * Register the kernel text and data with bootmem.
	 * Note that this can only be in node 0.
	 */
	reserve_bootmem_core(bdata, __pa(&__start), &__end - &__start);

	/*
	 * Reserve the page tables.  These are already in use,
	 * and can only be in node 0.
	 */
	reserve_bootmem_core(bdata, __pa(PAGING_TABLE),
			     ENT_PER_L1PT * sizeof(l1pte_t));

	/*
	 * And don't forget to reserve the allocator bitmap,
	 * which will be freed later.
	 */
	reserve_bootmem_core(bdata, bootmap_pfn << PAGE_SHIFT,
			     bootmap_pages << PAGE_SHIFT);

#if 1 // FIXME
	res_size = __pa(PAGING_TABLE) - PHYS_OFFSET;
#else
	/*
	 * Hmm... This should go elsewhere, but we really really need to
	 * stop things allocating the low memory; ideally we need a better
	 * implementation of GFP_DMA which does not assume that DMA-able
	 * memory starts at zero.
	 */
	if (machine_is_integrator() || machine_is_cintegrator())
		res_size = __pa(swapper_pg_dir) - PHYS_OFFSET;

	/*
	 * These should likewise go elsewhere.  They pre-reserve the
	 * screen memory region at the start of main system memory.
	 */
	if (machine_is_edb7211())
		res_size = 0x00020000;
	if (machine_is_p720t())
		res_size = 0x00014000;

#ifdef CONFIG_SA1111
	/*
	 * Because of the SA1111 DMA bug, we want to preserve our
	 * precious DMA-able memory...
	 */
	res_size = __pa(swapper_pg_dir) - PHYS_OFFSET;
#endif
#endif // FIXME
	if (res_size)
		reserve_bootmem_core(bdata, PHYS_OFFSET, res_size);
}

void alloc_init_page(unsigned long virt, unsigned long phys, unsigned long prot_l1, unsigned long prot)
{
	l1pte_t *l1ptep = l1pte_off(virt);
	l2pte_t *l2ptep;

	printf("enter alloc_init_page.\n");

	if ( *l1ptep == 0 ) {
		l2ptep = __alloc_bootmem(ENT_PER_L2PT * sizeof(l2pte_t), PAGE_SIZE, 0);

		printf("write l1ptep = %X, value = %X\n", l1ptep, __pa(l2ptep) | prot_l1);
		*l1ptep = __pa(l2ptep) | prot_l1;
		
		asm("mcr	p15, 0, %0, c7, c10, 1" : : "r"(l1ptep));
		asm("mcr	p15, 0, %0, c7, c10, 4" : : "r" (0));
	}
	l2ptep = (unsigned long*)((*l1ptep & 0xFFFFFC00) + ((virt & 0x000FF000) >> 10));

	printf("write l2ptep = %X, value = %X\n", l2ptep, (phys & 0xFFFFF000) | prot);
	*l2ptep = (phys & 0xFFFFF000) | prot;

	//set_pte(l2ptep, pfn_pte(phys >> PAGE_SHIFT, prot));
}

void __init bootmem_init(struct meminfo *mi)
{
	unsigned int start_pfn, bootmap_pfn;
	unsigned long mapsize;
	unsigned long pages;
	unsigned int start, end;
	unsigned int i;

	start = -1U;
	end = 0;

	printf("bootmem_init\n");

	// find_memend_and_nodes();
	for (i = 0; i < mi->nr_banks; i++ ) {
		unsigned long temp_s, temp_e;

		if (mi->bank[i].size == 0) {
			mi->bank[i].node = -1;
			continue;
		}

		temp_s = O_PFN_UP(mi->bank[i].start);
		temp_e = O_PFN_DOWN(mi->bank[i].start + mi->bank[i].size);

		printf("temp_s = %lX, temp_e = %lX\n", temp_s, temp_e);

		if (start > temp_s)
			start = temp_s;

		if (end < temp_e)
			end = temp_e;
	}

	printf("start = %lX, end = %lX\n", start, end);

	pages = end - start;
	mapsize = (pages+7)/8;
	mapsize = (mapsize + ~PAGE_MASK) & PAGE_MASK;
	mapsize >>= PAGE_SHIFT;
	printf("pages = %lX, mapsize = %lX\n", pages, mapsize);

	// find_bootmap_pfn();
	start_pfn   = V_PFN_UP(&__end);
	bootmap_pfn = 0;

	printf("bootmem_init2\n");

	for (i = 0; i < mi->nr_banks; i++ ) {
		unsigned int temp_s, temp_e;

		temp_s = O_PFN_UP(mi->bank[i].start);
		temp_e = O_PFN_DOWN(mi->bank[i].start + mi->bank[i].size);

		printf("temp_s = %lX, temp_e = %lX\n", temp_s, temp_e);

		if (temp_e < start_pfn)
			continue;

		if (temp_s < start_pfn)
			temp_s = start_pfn;

		if (temp_e - temp_s >= mapsize) {
			bootmap_pfn = temp_s;
			break;
		}
	}

	printf("start_pfn = %lX, bootmap_pfn = %lX\n", start_pfn, bootmap_pfn);

	printf("init_bootmem_core\n");
	init_bootmem_core(bootmap_pfn, start, end);
	printf("free_bootmem_bank\n");
	free_bootmem_bank(mi);

	printf("reserve_node_zero\n");
	reserve_node_zero(bootmap_pfn, mapsize);

	printf("bpgdata->node_bootmem_map = %lX\n", bpgdata.node_bootmem_map);

	printf("bootmem_end\n");
}

