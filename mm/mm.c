#include <mm/mm.h>
#include "lib/print.h"
#include "b_allocator.h"

int mm_early_init(void)
{
	b_allocator_init();
}
