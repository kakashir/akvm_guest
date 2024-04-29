#include <x86.h>
#include "config.h"

.balign	4096
.section ".data32","aw"
lv_2_page_table:
	.fill 4096, 1, 0
lv_3_page_table:
	.fill 4096, 1, 0
lv_4_page_table:
	.fill 4096, 1, 0
lv_5_page_table:
	.fill 4096, 1, 0

/* each idt itme has 8 bytes by 256 entries = 2048 KB */
.balign 8
idt32:
	.rept 32
	.4byte 0x00080000
	.4byte 0x00008f00
	.endr
	.rept 256 - 32
	.4byte 0x00080000
	.4byte 0x00008e00
	.endr
idt32_tab_desc:
	.2byte idt32_tab_desc - idt32
	.4byte idt32

.balign	4096
.section ".text32","ax"
.code32

.macro NULL_IDT_HANDLER from, to
	mov	$\from, %esi
	call null_idt32_handler
	iretl
.if \to - \from
	NULL_IDT_HANDLER "(\from+1)",\to
.endif
.endm

__one_null_idt32_handler_start:
NULL_IDT_HANDLER 0, 0
__one_null_idt32_handler_end:
NULL_IDT_HANDLER 1, 100
NULL_IDT_HANDLER 101, 200
NULL_IDT_HANDLER 201, 255

null_idt32_handler:
	/* DEAD! */
	mov $0xff, %eax
	mov $0xbad0deed, %edi
	/* %esi is exception/interrupt number from NULL_IDT_HANDLER */
	lea null_idt32_handler, %edx
	mov $0, %ecx
	vmcall
	retl

.global start32
start32:
	/* install null idt handler */
	lea  __one_null_idt32_handler_end - __one_null_idt32_handler_start, %ecx
	lea __one_null_idt32_handler_start, %eax
	sub %ecx, %eax
	lea idt32, %ebx
1:
	add %ecx, %eax
	mov %eax, %edx
	shr $0x10, %edx
	movw %ax, (%ebx)
	movw %dx, 0x6(%ebx)
	add $8, %ebx
	cmp $idt32_tab_desc, %ebx
	jnz 1b

	lidt idt32_tab_desc

	movl $X86_MSR_PAT_DEF_HIGH, %edx
	movl $X86_MSR_PAT_DEF_LOW, %eax
	movl $X86_MSR_PAT, %ecx
	wrmsr

/*
	It's not necessary to enable paging before
	move to ia32e mode, so below part can be skipped.
*/
/*
	call setup_tmp_protect_mode_page_table
	mov %cr4, %eax
	or $X86_CR4_PGE, %eax
	or $X86_CR4_PSE, %eax
	mov %eax, %cr4

	lea lv_2_page_table, %ecx
	mov %ecx, %cr3

	mov %cr0, %eax
	or $X86_CR0_PG, %eax
	mov %eax, %cr0
*/
	mov %cr0, %eax
	and $(~X86_CR0_PG), %eax
	mov %eax, %cr0

	/* prepare the temp ia32e mode page table */
	call setup_tmp_long_mode_page_table

	mov %cr4, %eax
	or $X86_CR4_PAE, %eax
	mov %eax, %cr4

	mov $X86_MSR_EFER, %ecx
	rdmsr
	or $X86_MSR_EFER_LME, %eax
	wrmsr

	lea lv_4_page_table, %eax
	mov %eax, %cr3

	/* enable ia32-e mode */
	mov %cr0, %eax
	or $X86_CR0_PG, %eax
	mov %eax, %cr0

	/* enable ia32-e's long mode by reload 64 bit cs selector */
	pushl $0x18
	pushl $start64_low
	retfl

setup_tmp_protect_mode_page_table:
	push %ebp
	mov %esp, %ebp

	lea lv_2_page_table, %edi
	mov $0x0, %ecx
	mov $0x183, %eax
1:
	mov %eax, (%edi, %ecx, 4)
	addl $X86_PAGE_SIZE_4M, %eax
	inc %ecx
	cmp $0x400, %ecx
	jnz 1b

	pop %ebp
	ret

setup_tmp_long_mode_page_table:
	push %ebp
	mov %esp, %ebp

	/* 4 1G entry for all low 4G memory */
	lea lv_3_page_table, %edi
	mov $0x0, %ecx
	mov $0x183, %eax
1:
	mov %eax, (%edi, %ecx, 8)
	addl $X86_PAGE_SIZE_1G, %eax
	inc %ecx
	cmp $0x4, %ecx
	jnz 1b

	/* kernel loading va mapping in lv 3*/
	lea lv_3_page_table, %edi
	mov $0x183, %eax
	mov $KERNEL_SIZE, %ebx
	mov $((KERNEL_64_LOAD_VA_ADDR >> (2 * 9 + 12)) & 0x1ff), %ecx
1:
	mov %eax, (%edi, %ecx, 8)
	add $X86_PAGE_SIZE_1G, %eax
	inc %ecx
	sub $X86_PAGE_SIZE_1G, %ebx
	cmp $0, %ebx
	jnz 1b

	/* 2 entries in level 4 for 1:1 and KERNEL_64_LOAD_VA_ADDR mapping */
	mov $lv_3_page_table + 0x003, %ecx
	lea lv_4_page_table, %esi
	mov %ecx, (%esi)
	mov $((KERNEL_64_LOAD_VA_ADDR >> (3 * 9 + 12)) & 0x1ff), %eax
	mov %ecx, (%esi, %eax, 8)

	/* 2 entries in level 5 for 1:1 and KERNEL_64_LOAD_VA_ADDR mapping */
	mov $lv_4_page_table + 0x003, %ecx
	lea lv_5_page_table, %esi
	mov %ecx, (%esi)
	mov $((KERNEL_64_LOAD_VA_ADDR >> (4 * 9 + 12)) & 0x1ff), %eax
	mov %ecx, (%esi, %eax, 8)

	pop %ebp
	ret
