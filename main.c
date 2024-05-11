#include <types.h>
#include <compiler.h>
#include <x86.h>
#include <entry.h>
#include <io.h>
#include "lib/print.h"
#include "mm/mm.h"

int start_kernel(void)
{
	setup_idt_table();
	setup_gdt();
	setup_tss();
	setup_idt_table_ist();

	mm_early_init();

	while(1) {;}
	return 1;
}
