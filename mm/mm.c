#include <mm/mm.h>
#include <io.h>
#include "lib/print.h"
#include "b_allocator.h"

struct phy_mem_range phy_mem_ranges[MAX_NR_PHY_MEM_RANGE];
int phy_mem_range_count;

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

int mm_early_init(void)
{
	init_phy_mem_ranges();
	b_allocator_init();
}
