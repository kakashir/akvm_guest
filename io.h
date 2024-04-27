#ifndef _IO_H
#define _IO_H

#include "types.h"
#include "entry.h"

void mmio_write(u64 addr, u64 size, u64 value);

#endif
