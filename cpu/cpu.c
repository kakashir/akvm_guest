#include <types.h>
#include <compiler.h>
#include <config.h>
#include <cpu/cpu.h>
#include <lib/print.h>

#include "x86.h"

int cpu_early_init(void)
{
	print("%s called\n", __func__);
}
