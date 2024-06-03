#ifndef _MM_MM_H
#define _MM_MM_H

#include <types.h>
#include <compiler.h>
#include <config.h>
#include <x86.h>

extern u64 early_page_table_root;
int mm_early_init(void);

struct phy_mem_range {
	u64 addr;
	u64 size;
	u64 type;
};

extern struct phy_mem_range phy_mem_ranges[MAX_NR_PHY_MEM_RANGE];
extern int phy_mem_range_count;

#define for_each_phy_mem_range(iter)				\
	for (iter = phy_mem_ranges;				\
	     iter < phy_mem_ranges + phy_mem_range_count;	\
	     ++iter)


#define pa_to_pfn(x) ((x) / (PAGE_SIZE))
#define pfn_to_pa(x) ((x) * PAGE_SIZE)

bool b_allocator_get_pages(u64 *addr, int n);
bool b_allocator_get_pages_top(u64 *addr, int n);
void b_allocator_free_pages(u64 addr, int n);

#define b_allocator_get_page(a) b_allocator_get_pages((u64*)a, 1)
#define b_allocator_get_page_top(a) b_allocator_get_pages_top((u64*)a, 1)
#define b_allocator_free_page(a) b_allocator_free_pages(b, 1)

#endif
