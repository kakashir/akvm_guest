#ifndef __VM_SERVICE_H
#define __VM_SERVICE_H

#define VM_SERVICE_DEBUG 0xfeULL
#define	VM_SERVICE_PANIC 0xffULL

struct vm_service_arg {
	union {
		unsigned long type;
		unsigned long ret;
	};
	unsigned long arg0;
	unsigned long arg1;
	unsigned long arg2;
	unsigned long arg3;
	unsigned long arg4;
	unsigned long arg5;
}__attribute__((packed));

#endif
