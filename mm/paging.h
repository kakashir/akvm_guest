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

#endif
