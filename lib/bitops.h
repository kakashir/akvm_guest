#ifndef _LIB_BITOPS_H
#define _LIB_BITOPS_H

#include <types.h>
#include <compiler.h>

#define BITS_PER_U64 (sizeof(u64) * 8)

static inline u64 bitmask64(int low, int high)
{
	return (high < 63 ? ((1ULL << (high + 1)) - 1) : -1LL) &
		~((1ULL << low) - 1);
}
#define BITS_ULL(l, h) \
	(((h) < 63 ? ((1ULL << ((h) + 1)) - 1) : -1LL) &	\
	 ~((1ULL << (l)) - 1))
#define BIT_ULL(x) (1ULL << x)

void bit_set(void *bitmap, u64 index, u64 size);
void bit_clear(void *bitmap, u64 index, u64 size);
bool bit_get(void *bitmap, u64 index);

/* forward search the u64 bitmap for first 1b*/
int bsf64(u64 v);

/* reverse search the u64 bitmap for first 1b*/
int bsr64(u64 v);

struct bs_context {
	int bit_count;
	int cur_bit_count;
	int start_bit;
	u64 *found;
	u64 *start;
	u64 *end;
};

static inline void bs_context_init(struct bs_context *bsc,
				   u64 *s, u64 *e, int n)
{
	bsc->bit_count = n;
	bsc->cur_bit_count = n;
	bsc->start = s;
	bsc->end = e;
}

bool search_bits(struct bs_context *bsc);
bool search_bits_reverse(struct bs_context *bsc);
#endif
