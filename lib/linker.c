#include "linker.h"
#include "ld.h"
#include "config.h"

#define KERNEL_SECTION_ITEM(s, e) \
	{						\
		(u64)&s, (u64)&e,			\
		KERNEL_64_LOAD_VA_ADDR + (u64)&s,	\
		KERNEL_64_LOAD_VA_ADDR + (u64)&e,	\
	}

struct kernel_section kernel_section[4] = {
	KERNEL_SECTION_ITEM(__kernel_16_start, __kernel_16_end),
	KERNEL_SECTION_ITEM(__kernel_32_start, __kernel_32_end),
	KERNEL_SECTION_ITEM(__kernel_64_low_start,  __kernel_64_low_end),
	KERNEL_SECTION_ITEM(__kernel_64_high_start, __kernel_64_high_end),
};

#undef KERNEL_SECTION_ITEM
