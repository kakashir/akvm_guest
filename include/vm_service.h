#ifndef __VM_SERVICE_H
#define __VM_SERVICE_H

#define VM_SERVICE_DEBUG 0xfeULL
#define	VM_SERVICE_PANIC 0xffULL

#define VM_SERVICE_IO 0x1
#define VM_SERVICE_IO_PORT_IN  0x1
#define VM_SERVICE_IO_PORT_OUT 0x2
#define VM_SERVICE_IO_MMIO_IN 0x3
#define VM_SERVICE_IO_MMIO_OUT 0x4

struct vm_service_arg {
	union {
		unsigned long type;
		unsigned long ret;
	};
	union {
		struct {
			unsigned long arg0;
			unsigned long arg1;
			unsigned long arg2;
			unsigned long arg3;
			unsigned long arg4;
			unsigned long arg5;
		} raw;

		struct {
			unsigned long sub_type;
			unsigned long addr;
			unsigned long size;
			unsigned long val;
		} io;
	};

}__attribute__((packed));

struct vm_service_arg;
void vm_service(struct vm_service_arg *arg);

#endif
