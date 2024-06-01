#ifndef __LIB_PRINT_H
#define __LIB_PRINT_H

#include <types.h>
#include <compiler.h>

int vsnprint(u8 *_buf, int _size, const char *_format, va_list _arg);

int print(const char *f, ...);

#endif
