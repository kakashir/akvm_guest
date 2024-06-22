#include <lib/print.h>
#include "x86.h"


int arch_cpu_early_init(void)
{
	print("%s called\n", __func__);
	return 0;
}
