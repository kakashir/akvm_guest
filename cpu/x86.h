#ifndef __CPU_X86_H
#define __CPU_X86_H

#include <types.h>
#include <compiler.h>
#include <config.h>
#include <cpu/cpu.h>

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

#define IDT_ENTRY_TYPE(_addr, _type, _ist) (struct idt64_entry) {	\
	.selector = KERNEL_CS_64,   \
	.p = 1,	     \
	.type = _type,  \
	.ist = _ist,	      \
	.offset0_15 = _addr & 0xffff, \
	.offset16_31 = (_addr >> 16) & 0xfffff,	\
	.offset32_63 = (_addr >> 32) & 0xffffffff, \
}

#define IDT_ENTRY_TRAP(addr, ist) IDT_ENTRY_TYPE(addr, IDT_TYPE_TRAP, ist)
#define IDT_ENTRY_INTR(addr, ist) IDT_ENTRY_TYPE(addr, IDT_TYPE_INTR, ist)

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

#define GDT_ENTRY(_type, _offset, _limit, _s, _l, _db) (struct gdt_entry) { \
	.seg_limit0_15 = (_limit) & 0xffff,			\
	.offset0_15 = (_offset) & 0xffff,		\
	.offset16_23 = ((_offset) >> 16) &0xff,	\
	.type = _type, \
	.s = _s,	  \
	.dpl = 0, \
	.p = 1, \
	.seg_limit16_19 = ((_limit) >> 16) & 0xf,	\
	.avl = 0, \
	.l = _l, \
	.db = _db, \
	.g = 1, \
	.offset24_31 = ((_offset) >> 24) & 0xffff	\
}

#define GDT_ENTRY_64(_type) GDT_ENTRY(_type, 0x0, 0xfffff, 1, 1, 0)

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

#define GDT_TSS_64(type, offset, limit) (struct gdt64_entry)	{ \
		.entry = GDT_ENTRY(type, offset, limit, 0, 0, 0), \
		.offset32_63 = ((offset) >> 32) & 0xffffffff, \
		.reserved = 0, \
	}

/* runtime supporting */
struct cpu_data {
	struct idt64_entry idt[MAX_VECTOR_NUMBER];

	/* 1 null (8byte)/ 1 code(8bytes)/1 data(8bytes)/1 64bit TSS(16 bytes)*/
	struct gdt_entry gdt[5];
	struct tss64_segment tss;

	void *kernel_stack_top;
	void *kernel_ist_stack_top;

	int pa_bits;
	int va_bits;
};

int arch_cpu_early_init(void);

#endif
