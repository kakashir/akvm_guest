#include <config.h>

.balign 4096
.section ".data64","aw"

.global idt_intr_entry_point_size
idt_intr_entry_point_size:
.quad  __idt_intr_one_end - __idt_intr_one_start

.balign 1
.section ".bss64","aw", @nobits
.section ".bss","aw", @nobits

.balign 8
.fill 4096, 1, 0
.global stack_top_64
stack_top_64:

.fill 4096, 7, 0
.global ist_stack_top_64
ist_stack_top_64:

.balign 8
.section ".text64","ax"
.code64

.macro DEFINE_EXCEP_HANDLER error_code vector asm_func_name
.global \asm_func_name
	\asm_func_name:
.if !\error_code
	push $0 /* fake error code for consistency */
.endif
	push $\vector
	jmp idt_handler_common
.endm

.macro DEFINE_INTR_HANDLER from, to
	.byte 0x6a, 0  /* push 0x0*/
	.byte 0x68, \from, 0, 0, 0  /* for 4 bytes opcode for from < 0x80 */
	jmp idt_handler_common
.if \to - \from
	DEFINE_INTR_HANDLER "(\from+1)", \to
.endif
.endm

idt_save_reg_context:
	push %r14
	push %r13
	push %r12
	push %r11
	push %r10
	push %r9
	push %r8
	push %rbp
	push %rdi
	push %rsi
	push %rdx
	push %rcx
	push %rbx
	push %rax
	jmp *%r15

idt_restore_reg_context:
	pop %rax
	pop %rbx
	pop %rcx
	pop %rdx
	pop %rsi
	pop %rdi
	pop %rbp
	pop %r8
	pop %r9
	pop %r10
	pop %r11
	pop %r12
	pop %r13
	pop %r14
	jmp *%r15


idt_handler_common:
	/*
		save r15 separately because it's used as ret addr from
		idt_setup_reg_context
	*/
	push %r15
	lea idt_save_reg_context_ret, %r15
	jmp idt_save_reg_context
idt_save_reg_context_ret:

	mov %rsp, %rdi
	call x86_excep_intr_common_handler

	lea idt_restore_reg_context_ret, %r15
	jmp idt_restore_reg_context
idt_restore_reg_context_ret:

	pop %r15
	add $(2 * 8), %rsp /* skip vector and error_code */
	iretq

DEFINE_EXCEP_HANDLER	0, 0, asm_excep_0
DEFINE_EXCEP_HANDLER	0, 1, asm_excep_1
DEFINE_EXCEP_HANDLER	0, 2, asm_excep_2
DEFINE_EXCEP_HANDLER	0, 3, asm_excep_3
DEFINE_EXCEP_HANDLER	0, 4, asm_excep_4
DEFINE_EXCEP_HANDLER	0, 5, asm_excep_5
DEFINE_EXCEP_HANDLER	0, 6, asm_excep_6
DEFINE_EXCEP_HANDLER	0, 7, asm_excep_7
DEFINE_EXCEP_HANDLER	1, 8, asm_excep_8
DEFINE_EXCEP_HANDLER	0, 9, asm_excep_9
DEFINE_EXCEP_HANDLER	1, 10, asm_excep_10
DEFINE_EXCEP_HANDLER	1, 11, asm_excep_11
DEFINE_EXCEP_HANDLER	1, 12, asm_excep_12
DEFINE_EXCEP_HANDLER	1, 13, asm_excep_13
DEFINE_EXCEP_HANDLER	1, 14, asm_excep_14
DEFINE_EXCEP_HANDLER	0, 16, asm_excep_16
DEFINE_EXCEP_HANDLER	1, 17, asm_excep_17
DEFINE_EXCEP_HANDLER	0, 18, asm_excep_18
DEFINE_EXCEP_HANDLER	0, 19, asm_excep_19
DEFINE_EXCEP_HANDLER	0, 20, asm_excep_20
DEFINE_EXCEP_HANDLER	1, 21, asm_excep_21

.global __idt_intr_one_start
__idt_intr_one_start:
DEFINE_INTR_HANDLER 32, 32
__idt_intr_one_end:
DEFINE_INTR_HANDLER 33, 132
DEFINE_INTR_HANDLER 133, 233
DEFINE_INTR_HANDLER 234, 255

.global vm_service
vm_service:
	push %rbp
	mov %rsp, %rbp
	push %rbx

	mov %rdi, %rbx
	mov 0*8(%rbx), %rax
	mov 1*8(%rbx), %rdi
	mov 2*8(%rbx), %rsi
	mov 3*8(%rbx), %rdx
	mov 4*8(%rbx), %rcx
	mov 5*8(%rbx), %r8
	mov 6*8(%rbx), %r9
	vmcall
	mov %rax, 0*8(%rbx)
	mov %rdi, 1*8(%rbx)
	mov %rsi, 2*8(%rbx)
	mov %rdx, 3*8(%rbx)
	mov %rcx, 4*8(%rbx)
	mov %r8,  5*8(%rbx)
	mov %r9,  6*8(%rbx)

	pop %rbx
	pop %rbp
	ret

.global load_idt
load_idt:
	push %rbp
	mov %rsp, %rbp
	lidt (%rdi)
	pop %rbp
	ret

.global load_gdt
load_gdt:
	push %rbp
	mov %rsp, %rbp
	lgdt (%rdi)
	pop %rbp
	ret

.global load_tss
load_tss:
	push %rbp
	mov %rsp, %rbp
	ltr %rdi
	pop %rbp
	ret

.global flush_segment_cache
flush_segment_cache:
	push %rbp
	mov %rsp, %rbp

	mov $KERNEL_DS_64, %eax
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %ss
	mov %eax, %fs
	mov %eax, %gs

	push $KERNEL_CS_64
	pushq $1f
	retfq
1:
	pop %rbp
	ret
