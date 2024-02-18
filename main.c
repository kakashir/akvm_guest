#include "types.h"
#include "x86.h"
#include "entry.h"

int start_kernel(void)
{
	setup_idt_table();
	setup_gdt();

	while(1);
	return 1;
}
