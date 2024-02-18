#include "config.h"
#include "entry.h"
#include "x86.h"
#include "vm_service.h"

/* 1 null (8byte)/ 1 code(8bytes)/1 data(8bytes)/1 64bit TSS(16 bytes)*/
static struct gdt_entry gdt[5];

#define GDT_ENTRY(_type, _offset, _l, _db) (struct gdt_entry) {	\
	.seg_limit0_15 = 0xffff,				\
	.offset0_15 = _offset & 0xffff,		\
	.offset16_23 = (_offset >> 16) &0xff,	\
	.type = _type, \
	.s = 1,	  \
	.dpl = 0, \
	.p = 1, \
	.seg_limit16_19 = 0xf, \
	.avl = 0, \
	.l = _l, \
	.db = _db, \
	.g = 1, \
	.offset24_31 = (_offset >> 24) & 0xffff \
}

#define GDT_ENTRY_64(_type) GDT_ENTRY(_type, 0x0, 1, 0)

#define IDT_ENTRY_TYPE(_addr, _type) (struct idt64_entry) {    \
	.selector = KERNEL_CS_64,   \
	.p = 1,	     \
	.type = _type,  \
	.offset0_15 = _addr & 0xffff, \
	.offset16_31 = (_addr >> 16) & 0xfffff,	\
	.offset32_63 = (_addr >> 32) & 0xffffffff, \
}

#define IDT_ENTRY_TRAP(addr) IDT_ENTRY_TYPE(addr, IDT_TYPE_TRAP)
#define IDT_ENTRY_INTR(addr) IDT_ENTRY_TYPE(addr, IDT_TYPE_INTR)

static struct idt64_entry idt[256];

void setup_idt_table(void)
{
	struct idt64_desc desc = {
		.size = sizeof(idt),
		.base = (unsigned long)idt,
	};
#undef ____SET_IDT
#define ____SET_IDT(x) idt[x] = IDT_ENTRY_TRAP((unsigned long)asm_excep_##x)
	____SET_IDT(0);
	____SET_IDT(1);
	____SET_IDT(2);
	____SET_IDT(3);
	____SET_IDT(4);
	____SET_IDT(5);
	____SET_IDT(6);
	____SET_IDT(7);
	____SET_IDT(8);
	____SET_IDT(9);
	____SET_IDT(10);
	____SET_IDT(11);
	____SET_IDT(12);
	____SET_IDT(13);
	____SET_IDT(14);
	/* ____SET_IDT(15); */
	____SET_IDT(16);
	____SET_IDT(17);
	____SET_IDT(18);
	____SET_IDT(19);
	____SET_IDT(20);
	____SET_IDT(21);
#undef ____SET_IDT

	for (int i = 0; i < MAX_VECTOR_NUMBER - INTR_VECTOR_BEGIN; ++i)
		idt[i + INTR_VECTOR_BEGIN] =
			IDT_ENTRY_INTR((unsigned long)__idt_intr_one_start +
				       i * idt_intr_entry_point_size);

	load_idt(&desc);
}

void x86_excep_intr_common_handler(struct inter_excep_regs *regs)
{
	struct vm_service_arg arg = {
		.type = VM_SERVICE_DEBUG,
		.arg0 = 0xbadULL,
		.arg1 = regs->vector,
		.arg2 = regs->rip,
		.arg3 = regs->rsp,
		.arg4 = (unsigned long)regs,
	};
	vm_service(&arg);
}

void setup_gdt(void)
{
	struct gdt64_desc desc = {
		.size = sizeof(gdt),
		.base = (unsigned long)gdt,
	};

	gdt[0] = (struct gdt_entry) { 0 };
	gdt[1] = GDT_ENTRY_64(GDT_TYPE_DATA);
	gdt[2] = GDT_ENTRY_64(GDT_TYPE_CODE);

	load_gdt(&desc);
	flush_segment_cache();
}
