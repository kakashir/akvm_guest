#ifndef _IO_H
#define _IO_H

#include <types.h>

void mmio_write(u64 addr, u64 size, u64 value);
u64 mmio_read64(u64 addr);

#endif
