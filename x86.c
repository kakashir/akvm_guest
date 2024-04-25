#include "config.h"
#include "entry.h"
#include "x86.h"
#include "vm_service.h"

/* 1 null (8byte)/ 1 code(8bytes)/1 data(8bytes)/1 64bit TSS(16 bytes)*/
static struct gdt_entry gdt[5];

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
#define GDT_TSS_64(type, offset, limit) (struct gdt64_entry)	{	\
		.entry = GDT_ENTRY(type, offset, limit, 0, 0, 0), \
		.offset32_63 = ((offset) >> 32) & 0xffffffff, \
		.reserved = 0, \
	}

#define IDT_ENTRY_TYPE(_addr, _type, _ist) (struct idt64_entry) {    \
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

static struct idt64_entry idt[256];

void setup_idt_table(void)
{
	struct idt64_desc desc = {
		.size = sizeof(idt),
		.base = (unsigned long)idt,
	};

#undef ____SET_IDT
#define ____SET_IDT(x) idt[x] = IDT_ENTRY_TRAP((unsigned long)asm_excep_##x, 0)
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
				       i * idt_intr_entry_point_size, 0);

	load_idt(&desc);
}

void setup_idt_table_ist(void)
{
#undef ____SET_IDT
#define ____SET_IDT(x, ist) \
	idt[x] = IDT_ENTRY_TRAP((unsigned long)asm_excep_##x, ist)

	____SET_IDT(2, EXCEP_2_IST);
	____SET_IDT(8, EXCEP_8_IST);
	____SET_IDT(18, EXCEP_18_IST);

#undef ____SET_IDT
}



void x86_excep_intr_common_handler(struct inter_excep_regs *regs)
{
	struct vm_service_arg arg = {
		.type = VM_SERVICE_DEBUG,
		.raw.arg0 = 0xbadULL,
		.raw.arg1 = regs->vector,
		.raw.arg2 = regs->rip,
		.raw.arg3 = regs->rsp,
		.raw.arg4 = (unsigned long)regs,
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
	gdt[sel_to_index(KERNEL_DS_64)] = GDT_ENTRY_64(GDT_TYPE_DATA);
	gdt[sel_to_index(KERNEL_CS_64)] = GDT_ENTRY_64(GDT_TYPE_CODE);

	load_gdt(&desc);
	flush_segment_cache();
}

static struct tss64_segment tss;

void setup_tss(void)
{
	struct gdt64_entry *p;
	unsigned long ist_stack_top = (unsigned long)&ist_stack_top_64;

	tss.io_map_base = offset_of(struct tss64_segment, io_map);
	tss.io_map = 0xff;
	tss.io_map_end = 0xff;
	tss.rsp[0] = (unsigned long)&stack_top_64;
	/* rsp1/rsp2 keep 0x0 as guard */
	for (int i = 6; i >= 0; --i) {
		tss.ist[i] = ist_stack_top;
		ist_stack_top -= 4096;
	}

	p = (struct gdt64_entry *)(gdt + sel_to_index(KERNEL_TSS_64));
	*p = GDT_TSS_64(GDT_TYPE_TSS,
			(unsigned long)&tss, sizeof(tss));
	load_tss(KERNEL_TSS_64);
}

unsigned long read_dr(int index)
{
	unsigned long val;

	switch(index) {
	case 0:
		asm volatile("mov %%dr0, %0\n\t":"=r"(val));
		break;
	case 1:
		asm volatile("mov %%dr1, %0\n\t":"=r"(val));
		break;
	case 2:
		asm volatile("mov %%dr2, %0\n\t":"=r"(val));
		break;
	case 3:
		asm volatile("mov %%dr3, %0\n\t":"=r"(val));
		break;
	case 6:
		asm volatile("mov %%dr6, %0\n\t":"=r"(val));
		break;
	case 7:
		asm volatile("mov %%dr7, %0\n\t":"=r"(val));
		break;
	default:
		val = 0;
	}

	return val;
}

void write_dr(int index, unsigned long val)
{
	switch(index) {
	case 0:
		asm volatile("mov %0, %%dr0\n\t"::"r"(val));
		break;
	case 1:
		asm volatile("mov %0, %%dr1\n\t"::"r"(val));
		break;
	case 2:
		asm volatile("mov %0, %%dr2\n\t"::"r"(val));
		break;
	case 3:
		asm volatile("mov %0, %%dr3\n\t"::"r"(val));
		break;
	case 6:
		asm volatile("mov %0, %%dr6\n\t"::"r"(val));
		break;
	case 7:
		asm volatile("mov %0, %%dr7\n\t"::"r"(val));
		break;
	default:
	}
}

unsigned long read_cr(int cr)
{
	unsigned long val;

	switch(cr) {
	case 0:
		asm volatile("mov %%cr0, %0\n\r":"=r"(val));
		break;
	case 2:
		asm volatile("mov %%cr2, %0\n\r":"=r"(val));
		break;
	case 3:
		asm volatile("mov %%cr3, %0\n\r":"=r"(val));
		break;
	case 4:
		asm volatile("mov %%cr4, %0\n\r":"=r"(val));
		break;
	case 8:
		asm volatile("mov %%cr8, %0\n\r":"=r"(val));
		break;
	default:
		;
	}

	return val;
}

void write_cr(int cr, unsigned long val)
{
	switch(cr) {
	case 0:
		asm volatile("mov %0, %%cr0\n\r"::"r"(val));
		break;
	case 2:
		asm volatile("mov %0, %%cr2\n\r"::"r"(val));
		break;
	case 3:
		asm volatile("mov %0, %%cr3\n\r"::"r"(val));
		break;
	case 4:
		asm volatile("mov %0, %%cr4\n\r"::"r"(val));
		break;
	case 8:
		asm volatile("mov %0, %%cr8\n\r"::"r"(val));
		break;
	default:
		;
	}
}
