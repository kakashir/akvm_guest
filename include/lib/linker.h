#ifndef LIB_LINKER_H
#define LIB_LINKER_H

#include <types.h>
#include <compiler.h>

struct kernel_section {
	u64 pa_start;
	u64 pa_end;
	u64 va_start;
	u64 va_end;
};

extern struct kernel_section kernel_section[4];

#define for_each_kernel_section(s)				\
	for (s = kernel_section;				\
	     s < &kernel_section[ARRAY_SIZE(kernel_section)];	\
	     ++s)

#endif
