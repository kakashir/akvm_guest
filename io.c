#include <io.h>
#include <vm_service.h>

void mmio_write(u64 addr, u64 size, u64 value)
{
	struct vm_service_arg arg = {
		.type = VM_SERVICE_IO,
		.io.sub_type = VM_SERVICE_IO_MMIO_OUT,
		.io.addr = addr,
		.io.size = size,
		.io.val = value,
	};

	vm_service(&arg);
}
