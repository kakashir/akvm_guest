#ifndef __ASM_X86_H
#define __ASM_X86_H

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
#define IDT_MAX_ENTRY_NR 256
#endif

#endif
