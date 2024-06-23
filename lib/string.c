#include <lib/string.h>

void swap_byte(u8* buf, int size)
{
	int e = size / 2;
	int t;

	for (int i = 0; i < e; ++i) {
		t = buf[i];
		buf[i] = buf[size - i - 1];
		buf[size - i - 1] = t;
	}
}

void dec2a(int d, u8 *buf)
{
	if (d >= 0 && d <= 9)
		*buf = '0' + d;
	else
		*buf = '?';
}

void hex2a(int d, u8 *buf)
{
	if (0x0 <= d && d <= 0x9)
		*buf = '0' + d;
	else if (0xa <= d && d <= 0xf)
		*buf = 'a' + (d - 0xa);
	else
		*buf = '?';
}

void* memset(void *addr, u8 byte, u64 size)
{
	u8 *p = addr;
	u8 *p_end = addr + size;
	u64* aligned_p;
	u64* aligned_end;

	if (size <= sizeof(u64)) {
		while(p < p_end)
			*p++ = byte;
		return addr;
	}

	aligned_p = (void*)ALIGN_DOWN((u64)p, sizeof(u64));
	aligned_end = (void*)ALIGN((u64)p_end, sizeof(u64));

	while(p < (u8*)aligned_p)
		*p++ = byte;

	if (aligned_p < aligned_end) {
		union {
			u8 u8[8];
			u16 u16[4];
			u32 u32[2];
			u64 u;
		} u;

		u.u8[1] = u.u8[0] = byte;
		u.u16[1] = u.u16[0];
		u.u32[1] = u.u32[0];

		do
			*aligned_p++ = u.u;
		while(aligned_p < aligned_end);
	}

	p = (u8*)aligned_end;
	while(p < p_end)
		*p++ = byte;

	return addr;
}

int memcmp(const void *l, const void *r, u64 size)
{
	const u8 *l8 = l;
	const u8 *r8 = r;
	int d;

	for (u64 i = 0; i < size; ++i) {
		d = l8[i] - r8[i];
		if (d)
			return d;
	}
	return 0;
}
