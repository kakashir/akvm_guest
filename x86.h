#ifndef __x86_H
#define __x86_H

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

#define IDT_TYPE_TRAP 0xf
#define IDT_TYPE_INTR 0xe
#define INTR_VECTOR_BEGIN 32
#define MAX_VECTOR_NUMBER 256

#ifndef __ASM__
#include "types.h"

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

void setup_idt_table(void);

#endif
#endif
