#ifndef __PAGING_H
#define __PAGING_H

#include <x86.h>
#include <config.h>

#define PAGE_SIZE X86_PAGE_SIZE
#define PAGE_PRESENT X86_PAGE_PRESENT
#define PAGE_WRITE X86_PAGE_WRITE
#define PAGE_GLOBAL  X86_PAGE_GLOBAL
#define PAGE_HUGE X86_PAGE_HUGE
#define PAGE_IDT_MAP_ATTR (PAGE_PRESENT | PAGE_WRITE)

#define PGD_LEVEL 4
#define PUD_LEVEL 3
#define PMD_LEVEL 2
#define PT_LEVEL 1
#define MAX_PAGE_LEVEL 4

#define VA_PAGE_LEVEL_INDEX X86_VA_PAGE_LEVEL_INDEX
#define PGD_INDEX(a) VA_PAGE_LEVEL_INDEX(a, PGD_LEVEL)
#define PUD_INDEX(a) VA_PAGE_LEVEL_INDEX(a, PUD_LEVEL)
#define PMD_INDEX(a) VA_PAGE_LEVEL_INDEX(a, PMD_LEVEL)
#define PT_INDEX(a) VA_PAGE_LEVEL_INDEX(a, PT_LEVEL)

#define PAGE_LEVEL_SIZE X86_PAGE_LEVEL_SIZE
#define PGD_SIZE PAGE_LEVEL_SIZE(PGD_LEVEL)
#define PUD_SIZE PAGE_LEVEL_SIZE(PUD_LEVEL)
#define PMD_SIZE PAGE_LEVEL_SIZE(PMD_LEVEL)
#define PT_SIZE PAGE_LEVEL_SIZE(PT_LEVEL)

#define PGD_HAS_HUGE 0
#define PUD_HAS_HUGE 1
#define PMD_HAS_HUGE 1
#define PAGE_LEVEL_HAS_HUG(l) ((1ULL << l) &			\
			       ((PGD_HAS_HUGE << PGD_LEVEL) |	\
				(PUD_HAS_HUGE << PUD_LEVEL) |	\
				(PMD_HAS_HUGE << PMD_LEVEL)))

/*
  HACK: this should come from cpuid 0x80000008, but hard code
  here for do memory mapping firstly. change this to use cpu
  init data after that is done.
*/
#define MAX_PA_BITS  39
#define PAGE_PA_MASK BITS_ULL(12, 39)
#define PAGE_PA(p) ((p) & PAGE_PA_MASK)

#define SET_PG_ROOT(r) write_cr(3, r)
#define GET_PG_ROOT(r) read_cr(3)

#define KERNEL_LOAD_VA(a) ((KERNEL_LOAD_VA_ADDR) + (a))

typedef x86_page_root pg_root_t;
typedef x86_pte_t pgd_t;
typedef x86_pte_t pud_t;
typedef x86_pte_t pmd_t;
typedef x86_pte_t pt_t;

#define va(p) ((typeof(p))(((u64)(p)) + (VA_LAYOUT_IDENTIFY_MAPPING_BEGIN)))
#define pa(v) ((typeof(p))(((u64)(v)) - (VA_LAYOUT_IDENTIFY_MAPPING_BEGIN)))

/*
  the low 4G memory is mapped 1:1 in early page table, so
  pa = va before identify memory mapping is done.
*/
static inline pt_t* pa_to_va_1_to_1(pt_t* pte)
{
	return pte;
}

/*
  add va -> pa mapping, pte points to root page table,
  allocate page table entires from below 4G top.
  pa_to_va: how to get va for one pa for memory access,
  for sharing code before/after identify mapping, use
  'pa_to_va_1_to_1' before identify mapping is done.
  if NULL then va() is used.
 */
s64 map_page(pt_t *pte, u64 va, u64 pa, u64 size, int level,
	     pt_t* pa_to_va(pt_t*));

/*
  This ONLY works when identify mapping is DONE,
  it's using va() to access page table entires!
 */
void dump_page_table(pg_root_t *pg_root, u64 addr);

#endif
