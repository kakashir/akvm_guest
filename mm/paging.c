#include <errno.h>
#include <mm/mm.h>
#include <lib/string.h>
#include <lib/print.h>
#include "paging.h"

static s64 map_normal_page(pt_t *pte, u64 pa)
{
	if (!(*pte & PAGE_PRESENT))
		*pte = PAGE_IDT_MAP_ATTR | pa;
	return PT_SIZE;
}

static s64 map_huge_page(pt_t *pte, u64 pa, u64 size, int level)
{
	if (!PAGE_LEVEL_HAS_HUG(level))
		return 0;

	if (size < PAGE_LEVEL_SIZE(level))
		return 0;

	if (!IS_ALIGN(pa, PAGE_LEVEL_SIZE(level)))
		return 0;

	*pte = PAGE_IDT_MAP_ATTR | PAGE_HUGE | pa;
	return PAGE_LEVEL_SIZE(level);
}

static void link_page_table(pt_t *pte, u64 new_next_low_level)
{
	*pte = PAGE_IDT_MAP_ATTR | new_next_low_level;
}

static inline pt_t* __mapping_pa_to_va(pt_t* pte)
{
	return va(pte);
}

s64 map_page(pt_t *pte, u64 va, u64 pa, u64 size, int level,
	     pt_t* pa_to_va(pt_t*))
{
	u64 mapped_size = 0;
	u64 next_va_end;
	u64 va_end;
	u64 old_va;
	pt_t *pte_e;

	old_va = va = ALIGN(va, PAGE_SIZE);
	/* - 1: don't align down to next boundary if va is already aligned */
	va_end = ALIGN_DOWN(va + size - 1, PAGE_SIZE);

	if (!pa_to_va)
		pa_to_va = __mapping_pa_to_va;

	if (level == PT_LEVEL)
		return map_normal_page(pa_to_va(pte + PT_INDEX(va)), pa);

	while (va < va_end) {
		pte_e = pa_to_va(pte + VA_PAGE_LEVEL_INDEX(va, level));
		if (!(*pte_e & PAGE_PRESENT)) {
			mapped_size = map_huge_page(pte_e, pa, size, level);
			if (!mapped_size) {
				pt_t new;

				if (!b_allocator_get_page_top(&new))
					return -ENOMEM;

				zero_memory(pa_to_va((pt_t*)new), PAGE_SIZE);
				link_page_table(pte_e, new);
			}
		}

		/*
		  it's huge may:
		  1. it's already there before try to install the mapping
		  2. above map_huge_pte() installed it just now.
		 */
		if (*pte_e & PAGE_HUGE) {
			mapped_size = PAGE_LEVEL_SIZE(level);
			goto next;
		}

		next_va_end = min(va_end,
				  ALIGN_DOWN(va, PAGE_LEVEL_SIZE(level - 1)));
		mapped_size = map_page((pt_t*)PAGE_PA(*pte_e),
				       va, pa, next_va_end - va, level - 1,
				       pa_to_va);
		if (mapped_size <= 0)
			break;
next:
		va += mapped_size;
		pa += mapped_size;
		size -= mapped_size;
	}

	return va - old_va;
}

void dump_page_table(pg_root_t *pg_root, u64 addr)
{
	pgd_t *pgd = NULL;
	pud_t *pud = NULL;
	pmd_t *pmd = NULL;
	pt_t *pt = NULL;

	pgd = va(pg_root) + PGD_INDEX(addr);
	if (!(*pgd & PAGE_PRESENT))
		goto exit;
	if (PGD_HAS_HUGE && *pgd & PAGE_HUGE)
		goto exit;

	pud = va((pud_t*)PAGE_PA(*pgd)) + PUD_INDEX(addr);
	if (!(*pud & PAGE_PRESENT))
		goto exit;
	if (PUD_HAS_HUGE && *pud & PAGE_HUGE)
		goto exit;

	pmd = va((pmd_t*)PAGE_PA(*pud)) + PMD_INDEX(addr);
	if (!(*pmd & PAGE_PRESENT))
		goto exit;
	if (PMD_HAS_HUGE && *pmd & PAGE_HUGE)
		goto exit;

	pt = va((pt_t*)PAGE_PA(*pmd)) + PT_INDEX(addr);

exit:
	if (pgd)
		print("va:0x%lx PGD: index:%d pte:0x%llx pte_val:0x%llx\n", addr, PGD_INDEX(addr), pgd, *pgd);
	if (pud)
		print("va:0x%lx PUD: index:%d pte:0x%llx pte_val:0x%llx\n", addr, PUD_INDEX(addr), pud, *pud);
	if (pmd)
		print("va:0x%lx PMD: index:%d pte:0x%llx pte_val:0x%llx\n", addr, PMD_INDEX(addr), pmd, *pmd);
	if (pt)
		print("va:0x%lx  PT: index:%d pte:0x%llx pte_val:0x%llx\n", addr, PT_INDEX(addr), pt, *pt);
}
