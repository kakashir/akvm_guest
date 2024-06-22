#include <types.h>
#include <compiler.h>
#include <x86.h>
#include <entry.h>
#include <io.h>
#include <lib/print.h>
#include <mm/mm.h>
#include <cpu/cpu.h>

int start_kernel(void)
{
	cpu_early_init();
	mm_early_init();
	print("Boot successfully!!\n");
	asm volatile ("int $0x40\n\r");
	while(1) {;}
	return 1;
}
