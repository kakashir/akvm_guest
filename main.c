#include <types.h>
#include <compiler.h>
#include <io.h>
#include <lib/print.h>
#include <lib/debug.h>
#include <mm/mm.h>
#include <cpu/cpu.h>

int start_kernel(void)
{
	int r;

	r = cpu_early_init();
	if (r)
		panic("Failed to do cpu_early_init()");
	r = mm_early_init();
	if (r)
		panic("Failed to do mm_early_init()");

	print("Boot successfully!!\n");
	asm volatile ("int $0x40\n\r");

	while(1) {;}
	return 0;
}
