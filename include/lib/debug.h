#ifndef __LIB_DEBUG_H
#define __LIB_DEBUG_H

#include "types.h"
#include "compiler.h"

#define debug1(a0) debug(a0, 0, 0, 0, 0, 0)
#define debug2(a0, a1) debug(a0, a1, 0, 0, 0, 0)
#define debug3(a0, a1, a2) debug(a0, a1, a2, 0, 0, 0)
#define debug4(a0, a1, a2, a3) debug(a0, a1, a2, a3, 0, 0)
#define debug5(a0, a1, a2, a3, a4) debug(a0, a1, a2, a3, a4, 0)
#define debug6(a0, a1, a2, a3, a4, a5) debug(a0, a1, a2, a3, a4, a5)

void debug_output(u64 arg0, u64 arg1, u64 arg2, u64 arg3, u64 arg4, u64 arg5);

#endif
