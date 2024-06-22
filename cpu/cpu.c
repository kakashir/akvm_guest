#include <types.h>
#include <compiler.h>
#include <config.h>
#include <cpu/cpu.h>
#include <lib/print.h>

#include "x86.h"

int cpu_early_init(void)
{
	arch_cpu_early_init();
	print("%s called\n", __func__);
}
