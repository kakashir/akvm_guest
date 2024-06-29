#ifndef __ASM_X86_H
#define __ASM_X86_H

#ifndef __ASM__

#include <asm/x86.h>
#include <lib/bitops.h>
#include "types.h"

typedef u64 x86_page_root;
typedef u64 x86_pte_t;

#endif

#define X86_MSR_PAT 0x277
#define X86_MSR_PAT_DEF_LOW 0x05010406ULL
#define X86_MSR_PAT_DEF_HIGH 0x00000007ULL

#define X86_MSR_EFER 0xc0000080
#define X86_MSR_EFER_LME_BIT 8
#define X86_MSR_EFER_LME 0x100

#define X86_CR0_PG_BIT 31
#define X86_CR0_PG 0x80000000

#define X86_CR4_PSE_BIT 4
#define X86_CR4_PSE 0x10
#define X86_CR4_PAE_BIT 5
#define X86_CR4_PAE 0x20
#define X86_CR4_PGE_BIT 7
#define X86_CR4_PGE 0x80

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

#ifndef __ASM__
#define X86_PAGE_ROOT_PCIDE BITS_ULL(0, 11)
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
#endif

#define X86_PAGE_SIZE_1G 0x40000000
#define X86_PAGE_SIZE_4M 0x400000

#define X86_PAGE_LEVEL_BITS 9
#define X86_PAGE_LEVEL_MASK ((1ULL << X86_PAGE_LEVEL_BITS) - 1)
#define X86_PAGE_SHIFT 12
#define X86_PAGE_SHIFT_MASK(l) (X86_PAGE_LEVEL_MASK << X86_PAGE_LEVEL_SHIFT(l))
#define X86_PAGE_LEVEL_SHIFT(l) (((l) - 1) * X86_PAGE_LEVEL_BITS +	\
				 X86_PAGE_SHIFT)
#define X86_PAGE_LEVEL_SIZE(l) (1ULL << X86_PAGE_LEVEL_SHIFT(l))
#define X86_PAGE_SIZE X86_PAGE_LEVEL_SIZE(1)
#define X86_PAGE_MASK (X86_PAGE_SIZE - 1)

#define X86_VA_PAGE_LEVEL_INDEX(a, l) (((a) >> X86_PAGE_LEVEL_SHIFT(l)) & \
				       X86_PAGE_LEVEL_MASK)

#define IDT_TYPE_TRAP 0xf
#define IDT_TYPE_INTR 0xe
#define INTR_VECTOR_BEGIN 32
#define MAX_VECTOR_NUMBER 256

#define GDT_TYPE_DATA 0x2
#define GDT_TYPE_CODE 0xa
#define GDT_TYPE_TSS 0x9

#ifndef __ASM__
unsigned long read_dr(int index);
void write_dr(int index, unsigned long val);
unsigned long read_cr(int cr);
void write_cr(int cr, unsigned long val);

void cpuid(int leaf, int sub_leaf,
	   int *eax, int *ebx, int *ecx, int *edx);
void cpuid_eax(int leaf, int sub_leaf, int *eax);
void cpuid_ebx(int leaf, int sub_leaf, int *ebx);
void cpuid_ecx(int leaf, int sub_leaf, int *ecx);
void cpuid_edx(int leaf, int sub_leaf, int *edx);

#define sel_to_index(x) ((x) >> 3)

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

struct panic_regs {
	u64 cr3;
	u64 cr2;
	u64 cr0;
	u64 ss;
	u64 cs;
	u64 rax;
	u64 rbx;
	u64 rcx;
	u64 rdx;
	u64 rsi;
	u64 rdi;
	u64 r8;
	u64 r9;
	u64 r10;
	u64 r11;
	u64 r12;
	u64 r13;
	u64 r14;
	u64 r15;
	u64 rflags;
	u64 rbp;
	u64 rsp;
};

/*This leads to unbalanced stack, know why you call this! */
#define generate_panic_regs_dump(r) \
	asm volatile("pushq %%rsp\n\r" \
		     "pushq %%rbp\n\r" \
		     "pushfq\n\r" \
		     "pushq %%r15\n\r" \
		     "pushq %%r14\n\r" \
		     "pushq %%r13\n\r" \
		     "pushq %%r12\n\r" \
		     "pushq %%r11\n\r" \
		     "pushq %%r10\n\r" \
		     "pushq %%r9\n\r" \
		     "pushq %%r8\n\r" \
		     "pushq %%rdi\n\r" \
		     "pushq %%rsi\n\r" \
		     "pushq %%rdx\n\r" \
		     "pushq %%rcx\n\r" \
		     "pushq %%rbx\n\r" \
		     "pushq %%rax\n\r" \
		     "movq %%cs, %%rax\n\r" \
		     "pushq %%rax\n\r" \
		     "movq %%ss, %%rax\n\r" \
		     "pushq %%rax\n\r" \
		     "movq %%cr0, %%rax\n\r" \
		     "pushq %%rax\n\r" \
		     "movq %%cr2, %%rax\n\r" \
		     "pushq %%rax\n\r" \
		     "movq %%cr3, %%rax\n\r" \
		     "pushq %%rax\n\r" \
		     "mov %%rsp, %0\n\r" \
		     :"=r"(r) \
		)

#define __DUMP_R(r, _r, _R) print("  "#_R": 0x%lx\n\r", r->_r)
#define show_panic_regs_dump(r) \
	__DUMP_R(r, rax, RAX);	\
	__DUMP_R(r, rbx, RBX);	\
	__DUMP_R(r, rcx, RCX);	\
	__DUMP_R(r, rdx, RDX);	\
	__DUMP_R(r, rsi, RSI);	\
	__DUMP_R(r, rdi, RDI);	\
	__DUMP_R(r, rbp, RBP);	\
	__DUMP_R(r, rsp, RSP);	\
	__DUMP_R(r, r8, R8);	\
	__DUMP_R(r, r9, R9);	\
	__DUMP_R(r, r10, R10);	\
	__DUMP_R(r, r11, R11);	\
	__DUMP_R(r, r12, R12);	\
	__DUMP_R(r, r13, R13);	\
	__DUMP_R(r, r14, R14);	\
	__DUMP_R(r, r15, R15);	  \
	__DUMP_R(r, rflags, RFLAGS);		\
	__DUMP_R(r, cr0, CR0);			\
	__DUMP_R(r, cr2, CR2);			\
	__DUMP_R(r, cr3, CR3);			\
	__DUMP_R(r, cs, SS);			\
	__DUMP_R(r, ss, CS);

#endif

#endif
