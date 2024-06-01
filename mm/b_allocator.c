#include <mm/mm.h>
#include <config.h>
#include <ld.h>
#include <io.h>
#include <x86.h>
#include <lib/bitops.h>
#include "b_allocator.h"
#include "lib/print.h"
#include "lib/linker.h"

#define PAGE_FREE 1
#define PAGE_USED 0
#define ADDR_4G 0x100000000ULL

/* 128k bitmap covers all low 4G memory space, 1 bit per page */
static u64 b_bitmap[ADDR_4G / PAGE_SIZE / (sizeof(u64) * 8)];

void __dump_bitmap(void)
{
	print("--------- dump begin ----------\n");
	for (int i = 0; i < 16; ++i)
		print("bitmap[%d] = 0x%lx\n", i, b_bitmap[i]);
	print("--------- dump end ----------\n");
}

static void mark_available_memory_range_free_used(u64 upper_limit,
						  u64 *free, u64 *used)
{
	const struct phy_mem_range *i;
	u64 b_start;
	u64 b_end;
	u64 p_addr;
	u64 size;

	for_each_phy_mem_range(i) {
		if (i->addr >= upper_limit)
			continue;

		size = MIN(i->size, upper_limit - i->addr);
		b_start = pa_to_pfn(i->addr);
		b_end = pa_to_pfn(i->addr + size + PAGE_SIZE - 1);
		size = b_end - b_start;

		if (i->type == HWCFG_MEMORY_TYPE_PHYSICAL) {
			bit_set(b_bitmap, b_start, size);
			*free += size;
		} else {
			bit_clear(b_bitmap, b_start, size);
			*used += size;
		}
	}
}

static void mark_kernel_range_used(u64 *used)
{
	u64 size;
	u64 b_end;
	u64 b_start;
	struct kernel_section *ks;

	for_each_kernel_section(ks) {
		b_start = pa_to_pfn(ks->pa_start);
		b_end = pa_to_pfn(ks->pa_end + PAGE_SIZE - 1);
		size = b_end - b_start;

		bit_clear(b_bitmap, b_start, size);
		*used += size;
	}
}

void b_allocator_init(void)
{
	u64 t_free = 0;
	u64 t_used = 0;

	/* upper_limit must <= 4G now */
	mark_available_memory_range_free_used(ADDR_4G, &t_free, &t_used);

	mark_kernel_range_used(&t_used);

	print("b_allocator: Below 4G: total %ldK free %ldK used %ldK\n",
	      KB((t_used + t_free) * PAGE_SIZE),
	      KB(t_free * PAGE_SIZE), KB(t_used * PAGE_SIZE));
}

static bool __b_allocator_get_page(u64 *addr)
{
	u64 *e = b_bitmap + ARRAY_SIZE(b_bitmap);
	u64 *s = b_bitmap;
	int b;

	for (; s != e; ++s) {
		b = bsf64(*s);
		if (!b && !*s)
			continue;

		bit_clear(s, b, 1);
		*addr = pfn_to_pa((s - b_bitmap) * BITS_PER_U64 + b);
		return true;
	}

	return false;
}

static bool __b_allocator_get_page_top(u64 *addr)
{
	u64 *e = b_bitmap + (ARRAY_SIZE(b_bitmap) - 1);
	u64 *s = b_bitmap;
	int b;

	for (; e >= s; --e) {
		b = bsr64(*e);
		if (!b && !*e)
			continue;

		bit_clear(e, b, 1);
		*addr = pfn_to_pa((e - s) * BITS_PER_U64 + b);
		return true;
	}

	return false;
}

static void __b_allocator_free_page(u64 addr)
{
	if (addr >= ADDR_4G)
		return;

	if (PAGE_FREE == bit_get(b_bitmap, pa_to_pfn(addr)))
		return;

	bit_set(b_bitmap, pa_to_pfn(addr), 1);
}

static bool __b_allocator_get_pages(u64 *addr, int n, bool top)
{
	struct bs_context bsc;
	u64 free_addr;
	bool r;

	if (!n)
		return false;

	if (!top) {
		bs_context_init(&bsc,
				b_bitmap,
				b_bitmap + ARRAY_SIZE(b_bitmap) - 1, n);
		r = search_bits(&bsc);
	} else {
		bs_context_init(&bsc,
				b_bitmap,
				b_bitmap + ARRAY_SIZE(b_bitmap), n);
		r = search_bits_reverse(&bsc);
	}
	if (!r)
		return r;

	bit_clear(bsc.found, bsc.start_bit, n);
	free_addr = pfn_to_pa(bsc.start_bit +
			      (bsc.found - bsc.start) * BITS_PER_U64);
	*addr = free_addr;
	return true;
}

bool b_allocator_get_pages(u64 *addr, int n)
{
	if (n == 1)
		return __b_allocator_get_page(addr);
	return __b_allocator_get_pages(addr, n, false);
}

bool b_allocator_get_pages_top(u64 *addr, int n)
{
	if (n == 1)
		return __b_allocator_get_page_top(addr);
	return __b_allocator_get_pages(addr, n, true);
}

void b_allocator_free_pages(u64 addr, int n)
{
	if (addr >= ADDR_4G)
		return;

	if (n == 1)
		return __b_allocator_free_page(addr);

	bit_set(b_bitmap, pa_to_pfn(addr), n);
}
