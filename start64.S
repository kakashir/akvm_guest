#include "config.h"

.balign 4096
.section ".text64_low","ax"
.code64

.global start64_low
start64_low:
	lea start64(%rip), %rcx
	call *%rcx

.balign 8
.section ".text64","ax"
.code64

.global start64
start64:
1:
	mov %ss, %rax
	push %rax
	pushq $stack_top_64
	pushfq
	mov %cs, %rcx
	push %rcx
	lea start_kernel(%rip), %rdx
	push %rdx
	iretq
