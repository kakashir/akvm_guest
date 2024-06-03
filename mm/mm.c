#include <mm/mm.h>
#include <io.h>
#include <errno.h>
#include <lib/print.h>
#include <lib/string.h>
#include <lib/linker.h>
#include "b_allocator.h"
#include "paging.h"

struct phy_mem_range phy_mem_ranges[MAX_NR_PHY_MEM_RANGE];
int phy_mem_range_count;

pg_root_t *kernel_page_table;

static void init_phy_mem_ranges(void)
{
	int i = phy_mem_range_count;
	u64 size;

	mmio_write(HWCFG_REG_TYPE, 8, HWCFG_MEMORY_INFO);
	for (;;) {
		size = mmio_read64(HWCFG_REG_DATA_1);
		if (!size)
			break;

		if (i >= MAX_NR_PHY_MEM_RANGE) {
			print("WARN: MAX_NR_PHY_MEM_RANGE(%d) reached, not all physical memory are usable\n",
			      MAX_NR_PHY_MEM_RANGE);
			break;
		}

		phy_mem_ranges[i].addr = mmio_read64(HWCFG_REG_DATA_0);
		phy_mem_ranges[i].type = mmio_read64(HWCFG_REG_DATA_2);
		phy_mem_ranges[i].size = size;
		++i;
		mmio_write(HWCFG_REG_CTL, 8, HWCFG_CTL_NEXT_ITEM);
	}

	phy_mem_range_count = i;
}

static int init_kernel_page_mapping(void)
{
	const struct phy_mem_range *p;
	const struct kernel_section *ks;
	s64 r;

	if (!b_allocator_get_page_top(&kernel_page_table))
		return -ENOMEM;

	for_each_phy_mem_range(p)
		r = map_page(kernel_page_table,  va(p->addr), p->addr, p->size,
			     MAX_PAGE_LEVEL, pa_to_va_1_to_1);
	if (r > 0)
		for_each_kernel_section(ks)
			r = map_page(kernel_page_table,
				     KERNEL_LOAD_VA(ks->pa_start),
				     ks->pa_start, ks->pa_end - ks->pa_start,
				     MAX_PAGE_LEVEL, pa_to_va_1_to_1);
	if (r > 0) {
		SET_PG_ROOT((unsigned long)kernel_page_table);
		print("Kernel page table root: 0x%lx\n", kernel_page_table);
	}

	return r;
}

int mm_early_init(void)
{
	init_phy_mem_ranges();
	b_allocator_init();
	return init_kernel_page_mapping();
}
