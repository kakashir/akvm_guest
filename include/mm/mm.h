#ifndef _MM_MM_H
#define _MM_MM_H

#include <types.h>
#include <compiler.h>
#include <x86.h>

#define PAGE_SIZE X86_PAGE_SIZE

extern u64 early_page_table_root;
int mm_early_init(void);

#define pa_to_pfn(x) ((x) / (PAGE_SIZE))
#define pfn_to_pa(x) ((x) * PAGE_SIZE)

bool b_allocator_get_pages(u64 *addr, int n);
bool b_allocator_get_pages_top(u64 *addr, int n);
void b_allocator_free_pages(u64 addr, int n);

#define b_allocator_get_page(a) b_allocator_get_pages(a, 1)
#define b_allocator_get_page_top(a) b_allocator_get_pages_top(a, 1)
#define b_allocator_free_page(a) b_allocator_free_pages(b, 1)

#endif
