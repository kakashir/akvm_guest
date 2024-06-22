#ifndef __x86_H
#define __x86_H

#include <asm/x86.h>
#include <lib/bitops.h>
#include "types.h"

typedef u64 x86_page_root;
typedef u64 x86_pte_t;
#define X86_PAGE_ROOT_PCIDE BITS_ULL(0, 11)

#define X86_PAGE_PRESENT_BIT 0
#define X86_PAGE_WRITE_BIT 1
#define X86_PAGE_USER_BIT 2
#define X86_PAGE_PWT_BIT 3
#define X86_PAGE_PCD_BIT 4
#define X86_PAGE_ACCESS_BIT 5
#define X86_PAGE_DIRTY_BIT 6
#define X86_PAGE_PAT_BIT 7
#define X86_PAGE_HUGE_BIT 7
#define X86_PAGE_GLOBAL_BIT 8
#define X86_PAGE_HUGE_PAT_BIT 12
#define X86_PAGE_NON_EXEC_BIT 63

#define X86_PAGE_PRESENT BIT_ULL(X86_PAGE_PRESENT_BIT)
#define X86_PAGE_WRITE BIT_ULL(X86_PAGE_WRITE_BIT)
#define X86_PAGE_USER BIT_ULL(X86_PAGE_USER_BIT)
#define X86_PAGE_PWT BIT_ULL(X86_PAGE_PWT_BIT)
#define X86_PAGE_PCD BIT_ULL(X86_PAGE_PCD_BIT)
#define X86_PAGE_ACCESS BIT_ULL(X86_PAGE_ACCESS_BIT)
#define X86_PAGE_DIRTY BIT_ULL(X86_PAGE_DIRTY_BIT)
#define X86_PAGE_PAT BIT_ULL(X86_PAGE_PAT_BIT)
#define X86_PAGE_HUGE BIT_ULL(X86_PAGE_HUGE_BIT)
#define X86_PAGE_GLOBAL BIT_ULL(X86_PAGE_GLOBAL_BIT)
#define X86_PAGE_HUGE_PAT BIT_ULL(X86_PAGE_HUGE_PAT_BIT)
#define X86_PAGE_NON_EXEC BIT_ULL(X86_PAGE_NON_EXEC_BIT)

struct inter_excep_regs {
	u64 rax;
	u64 rbx;
	u64 rcx;
	u64 rdx;
	u64 rsi;
	u64 rdi;
	u64 rbp;
	u64 r8;
	u64 r9;
	u64 r10;
	u64 r11;
	u64 r12;
	u64 r13;
	u64 r14;
	u64 r15;
	u64 vector;
	u64 error_code;
	u64 rip;
	u64 cs;
	u64 rflags;
	u64 rsp;
	u64 ss;
};

#endif
