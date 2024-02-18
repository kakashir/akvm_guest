#include "config.h"
#include "entry.h"
#include "x86.h"
#include "vm_service.h"

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
	vm_service(VM_SERVICE_PANIC, 0xbadULL,
		   regs->vector, regs->rip, 0, 0);
}
