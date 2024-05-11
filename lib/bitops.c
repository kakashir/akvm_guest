#include "bitops.h"

static void bit_op(void *bitmap, bool set, u64 index, u64 size)
{
	u64 *b = bitmap;
	u64 set_size;
	u64 mask;
	int m_b;
	int m_e;
	int i;

	for(; size > 0; size -= set_size, index += set_size) {
		set_size = MIN(ALIGN_DOWN(index, BITS_PER_U64) - index, size);
		m_b = index & (BITS_PER_U64 - 1);
		m_e = m_b + set_size - 1;
		mask = BITS_ULL(m_b, m_e);
		i = index / BITS_PER_U64;

		if (set)
			b[i] |= mask;
		else
			b[i] &= ~mask;
	}
}

void bit_clear(void *bitmap, u64 index, u64 size)
{
	bit_op(bitmap, false, index, size);
}

void bit_set(void *bitmap, u64 index, u64 size)
{
	bit_op(bitmap, true, index, size);
}

bool bit_get(void *bitmap, u64 index)
{
	u64 *b = bitmap;

	b += index / BITS_PER_U64;
	return !!(*b & (1ULL << (index & (BITS_PER_U64 - 1))));
}

int bsf64(u64 v)
{
	u64 r;
	asm volatile("bsf %1, %0" :"=r"(r) :"m"(v):);
	return (int)r;
}

int bsr64(u64 v)
{
	u64 r;
	asm volatile("bsr %1, %0" :"=r"(r) :"m"(v):);
	return (int)r;
}

bool search_bits(struct bs_context *bsc)
{
	bool new_search = true;
	u64 *s = bsc->start;
	u64 *e = bsc->end;
	u64 v;

	for (;s != e; ++s)
		for (int i = 0, v = *s; i < 64; ++i) {
			if (!bsc->cur_bit_count)
				return true;

			if (!(v & (1ULL << i))) {
				new_search = true;
				bsc->cur_bit_count = bsc->bit_count;
				continue;
			}

			if (new_search) {
				new_search = false;
				bsc->start_bit = i;
				bsc->found = s;
			}
			--bsc->cur_bit_count;
		}
	return false;
}

bool search_bits_reverse(struct bs_context *bsc)
{
	u64 *s = bsc->start;
	u64 *e = bsc->end;
	u64 v;

	if (!bsc->cur_bit_count)
		return false;

	for (; e != s; --e)
		for (int i = 63, v = *e; i >= 0; --i) {
			if (!(v & (1ULL << i))) {
				bsc->cur_bit_count = bsc->bit_count;
				continue;
			}

			if (!--bsc->cur_bit_count) {
				bsc->found = e;
				bsc->start_bit = i;
				return true;
			}
		}
	return false;
}
