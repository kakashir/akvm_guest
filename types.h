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

#define offset_of(c, o) \
	((unsigned long)(&(((typeof(c) *)0)->o)))

#endif
