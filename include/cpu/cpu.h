#ifndef __CPU_CPU_H
#define __CPU_CPU_H

#include <types.h>
#include <compiler.h>
#include <config.h>
#include <asm/x86.h>

int cpu_early_init(void);
int cpu_pa_bits(void);

typedef void (*excep_intr_handler)(struct inter_excep_regs *regs);

void register_excep_intr_handler(unsigned int vector,
				 excep_intr_handler handler);
excep_intr_handler get_excep_intr_handler(unsigned int vector);

#endif
