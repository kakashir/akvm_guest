#include <types.h>
#include <compiler.h>
#include <x86.h>
#include <entry.h>

int start_kernel(void)
{
	setup_idt_table();
	setup_gdt();
	setup_tss();
	setup_idt_table_ist();

	while(1) {
		asm volatile("int $0x40");
	}
	return 1;
}
