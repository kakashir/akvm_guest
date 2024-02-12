.section ".data32","aw"
	.balign	4096
lv_2_page_table:
	/* at least 1 page or 4M level mapping + 2 level paging */
	.fill 4096, 1, 0
lv_4_5_page_table:
	/* at least 3 pages	for 1G level mapping + 5 level paging*/
	.fill 4096, 3, 0

	/* each idt itme has 8 bytes by 256 entries = 2048 KB */
	.balign	8
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

.section ".text32","ax"
.code32
.balign 4

.macro NULL_IDT_HANDLER from, to
	mov	$\from, %esi
	call null_idt32_handler
	iretl
.if \to - \from
	NULL_IDT_HANDLER "(\from+1)",\to
.endif
.endm

.balign 1
__one_null_idt32_handler_start:
NULL_IDT_HANDLER 0, 0
__one_null_idt32_handler_end:
NULL_IDT_HANDLER 1, 100
NULL_IDT_HANDLER 101, 200
NULL_IDT_HANDLER 201, 255

.balign 4
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
	int $0x3
