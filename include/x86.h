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

struct idt64_entry {
	unsigned int offset0_15:16;
	unsigned int selector:16;
	unsigned int ist:3;
	unsigned int zero:5;
	unsigned int type:4;
	unsigned int zero1:1;
	unsigned int dpl:2;
	unsigned int p:1;
	unsigned int offset16_31:16;
	unsigned int offset32_63;
	unsigned int reserved;
} __attribute__((packed));

struct idt64_desc {
	unsigned short int size;
	unsigned long base;
} __attribute__((packed));

struct gdt_entry {
	unsigned int seg_limit0_15:16;
	unsigned int offset0_15:16;
	unsigned int offset16_23:8;
	unsigned int type:4;
	unsigned int s:1;
	unsigned int dpl:2;
	unsigned int p:1;
	unsigned int seg_limit16_19:4;
	unsigned int avl:1;
	unsigned int l:1;
	unsigned int db:1;
	unsigned int g:1;
	unsigned int offset24_31:8;
} __attribute__((packed));

struct gdt64_entry {
	struct gdt_entry entry;
	unsigned int offset32_63;
	unsigned int reserved;
} __attribute__((packed));

struct gdt64_desc {
	unsigned short int size;
	unsigned long base;
} __attribute__((packed));

struct tss64_segment {
	u32 reserved;
	u64 rsp[3];
	u32 reserved1[2];
	u64 ist[7];
	u32 reserved2[2];
	u16 reserved3;
	u16 io_map_base;
	u8 intr_redir_map[32];
	u8 io_map;
	u8 io_map_end;
} __attribute__((packed));

void setup_idt_table(void);
void setup_gdt(void);
void setup_tss(void);
void setup_idt_table_ist(void);
unsigned long read_dr(int index);
void write_dr(int index, unsigned long val);
unsigned long read_cr(int cr);
void write_cr(int cr, unsigned long val);

#define sel_to_index(x) ((x) >> 3)

#endif
