#ifndef __TYPES_H
#define __TYPES_H

typedef unsigned long u64;
typedef unsigned int u32;
typedef unsigned short int u16;
typedef unsigned char u8;

typedef long s64;
typedef int s32;
typedef short int s16;
typedef char s8;

typedef u8 bool;
#define true 1
#define false 0

#define offset_of(c, o) \
	((unsigned long)(&(((typeof(c) *)0)->o)))
#define ALIGN(i, n) ((i) & ~((n) - 1))
#define ALIGN_DOWN(i, n) ALIGN((i) + (n), n)
#endif
