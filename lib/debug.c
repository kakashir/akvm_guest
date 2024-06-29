#include <vm_service.h>
#include <lib/debug.h>
#include <lib/print.h>
#include <cpu/cpu.h>

void debug_output(u64 arg0, u64 arg1, u64 arg2, u64 arg3, u64 arg4, u64 arg5)
{
	struct vm_service_arg arg = {
		.type = VM_SERVICE_DEBUG,
		.raw.arg0 = arg0,
		.raw.arg1 = arg1,
		.raw.arg2 = arg2,
		.raw.arg3 = arg3,
		.raw.arg4 = arg4,
		.raw.arg5 = arg5,
	};
	vm_service(&arg);
}

static void hang(void)
{
	print("System hang.\n");
	while(1){;}
}

void panic(const char *msg)
{
	struct panic_regs *regs;

	generate_panic_regs_dump(regs);
	print("[PANIC]: %s\n\r", msg);
	show_panic_regs_dump(regs);

	hang();
}
