#include <types.h>
#include <compiler.h>
#include <config.h>
#include <cpu/cpu.h>
#include <lib/print.h>

#include "x86.h"

int cpu_early_init(void)
{
	return arch_cpu_early_init();
}
