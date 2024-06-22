#include <config.h>
#include <entry.h>
#include <x86.h>
#include <vm_service.h>

void x86_excep_intr_common_handler(struct inter_excep_regs *regs)
{
	struct vm_service_arg arg = {
		.type = VM_SERVICE_DEBUG,
		.raw.arg0 = 0xbadULL,
		.raw.arg1 = regs->vector,
		.raw.arg2 = regs->rip,
		.raw.arg3 = regs->rsp,
		.raw.arg4 = (unsigned long)regs,
	};
	vm_service(&arg);
}

unsigned long read_dr(int index)
{
	unsigned long val;

	switch(index) {
	case 0:
		asm volatile("mov %%dr0, %0\n\t":"=r"(val));
		break;
	case 1:
		asm volatile("mov %%dr1, %0\n\t":"=r"(val));
		break;
	case 2:
		asm volatile("mov %%dr2, %0\n\t":"=r"(val));
		break;
	case 3:
		asm volatile("mov %%dr3, %0\n\t":"=r"(val));
		break;
	case 6:
		asm volatile("mov %%dr6, %0\n\t":"=r"(val));
		break;
	case 7:
		asm volatile("mov %%dr7, %0\n\t":"=r"(val));
		break;
	default:
		val = 0;
	}

	return val;
}

void write_dr(int index, unsigned long val)
{
	switch(index) {
	case 0:
		asm volatile("mov %0, %%dr0\n\t"::"r"(val));
		break;
	case 1:
		asm volatile("mov %0, %%dr1\n\t"::"r"(val));
		break;
	case 2:
		asm volatile("mov %0, %%dr2\n\t"::"r"(val));
		break;
	case 3:
		asm volatile("mov %0, %%dr3\n\t"::"r"(val));
		break;
	case 6:
		asm volatile("mov %0, %%dr6\n\t"::"r"(val));
		break;
	case 7:
		asm volatile("mov %0, %%dr7\n\t"::"r"(val));
		break;
	default:
	}
}

unsigned long read_cr(int cr)
{
	unsigned long val;

	switch(cr) {
	case 0:
		asm volatile("mov %%cr0, %0\n\r":"=r"(val));
		break;
	case 2:
		asm volatile("mov %%cr2, %0\n\r":"=r"(val));
		break;
	case 3:
		asm volatile("mov %%cr3, %0\n\r":"=r"(val));
		break;
	case 4:
		asm volatile("mov %%cr4, %0\n\r":"=r"(val));
		break;
	case 8:
		asm volatile("mov %%cr8, %0\n\r":"=r"(val));
		break;
	default:
		;
	}

	return val;
}

void write_cr(int cr, unsigned long val)
{
	switch(cr) {
	case 0:
		asm volatile("mov %0, %%cr0\n\r"::"r"(val));
		break;
	case 2:
		asm volatile("mov %0, %%cr2\n\r"::"r"(val));
		break;
	case 3:
		asm volatile("mov %0, %%cr3\n\r"::"r"(val));
		break;
	case 4:
		asm volatile("mov %0, %%cr4\n\r"::"r"(val));
		break;
	case 8:
		asm volatile("mov %0, %%cr8\n\r"::"r"(val));
		break;
	default:
		;
	}
}

void cpuid(int leaf, int sub_leaf,
	   int *eax, int *ebx, int *ecx, int *edx)
{
	asm volatile("cpuid"
		     :"=a"(*eax),"=b"(*ebx),"=c"(*ecx),"=d"(*edx)
		     :"a"(leaf),"c"(sub_leaf));
}

void cpuid_eax(int leaf, int sub_leaf, int *eax)
{
	asm volatile("cpuid"
		     :"=a"(*eax)
		     :"a"(leaf),"c"(sub_leaf));
}

void cpuid_ebx(int leaf, int sub_leaf, int *ebx)
{
	asm volatile("cpuid"
		     :"=b"(*ebx)
		     :"a"(leaf),"c"(sub_leaf));
}

void cpuid_ecx(int leaf, int sub_leaf, int *ecx)
{
	asm volatile("cpuid"
		     :"=c"(*ecx)
		     :"a"(leaf),"c"(sub_leaf));
}

void cpuid_edx(int leaf, int sub_leaf, int *edx)
{
	asm volatile("cpuid"
		     :"=d"(*edx)
		     :"a"(leaf),"c"(sub_leaf));
}
