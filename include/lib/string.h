#ifndef __LIB_STRING_H
#define __LIB_STRING_H

#include <types.h>
#include <compiler.h>

void swap_byte(u8* buf, int size);
void dec2a(int d, u8 *buf);
void hex2a(int d, u8 *buf);
void* memset(void* addr, u8 byte, u64 size);
#define zero_memory(a, s) memset(a, 0, s);

#endif
