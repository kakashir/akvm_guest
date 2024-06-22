#include <lib/print.h>
#include "x86.h"
#include <x86.h>
#include <vm_service.h>

#include "entry.h"

static struct cpu_data boot_cpu;

static void setup_idt64_table_ist(struct idt64_entry *idt)
{
#undef ____SET_IDT
#define ____SET_IDT(x, ist) \
	idt[x] = IDT_ENTRY_TRAP((unsigned long)asm_excep_##x, ist)

	____SET_IDT(2, EXCEP_2_IST);
	____SET_IDT(8, EXCEP_8_IST);
	____SET_IDT(18, EXCEP_18_IST);

#undef ____SET_IDT
}

static  void setup_idt64_table(struct idt64_entry *idt, int size)
{
	struct idt64_desc desc = {
		.size = size,
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

static void setup_gdt(struct gdt_entry *gdt, int size)
{
	struct gdt64_desc desc = {
		.size = size,
		.base = (unsigned long)gdt,
	};

	gdt[0] = (struct gdt_entry) { 0 };
	gdt[sel_to_index(KERNEL_DS_64)] = GDT_ENTRY_64(GDT_TYPE_DATA);
	gdt[sel_to_index(KERNEL_CS_64)] = GDT_ENTRY_64(GDT_TYPE_CODE);

	load_gdt(&desc);
	flush_segment_cache();
}

static void setup_tss(struct gdt_entry *gdt, struct tss64_segment *tss,
		      int tss_size)
{
	struct gdt64_entry *p;
	unsigned long ist_stack_top = (unsigned long)&ist_stack_top_64;

	tss->io_map_base = offset_of(struct tss64_segment, io_map);
	tss->io_map = 0xff;
	tss->io_map_end = 0xff;
	tss->rsp[0] = (unsigned long)&stack_top_64;
	/* rsp1/rsp2 keep 0x0 as guard */
	for (int i = 6; i >= 0; --i) {
		tss->ist[i] = ist_stack_top;
		ist_stack_top -= 4096;
	}

	p = (struct gdt64_entry *)(gdt + sel_to_index(KERNEL_TSS_64));
	*p = GDT_TSS_64(GDT_TYPE_TSS,
			(unsigned long)tss, tss_size);
	load_tss(KERNEL_TSS_64);
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

int arch_cpu_early_init(void)
{
	setup_idt64_table(boot_cpu.idt, sizeof(boot_cpu.idt));
	setup_gdt(boot_cpu.gdt, sizeof(boot_cpu.gdt));
	setup_tss(boot_cpu.gdt, &boot_cpu.tss, sizeof(boot_cpu.tss));
	setup_idt64_table_ist(boot_cpu.idt);
	print("%s called\n", __func__);
	return 0;
}
