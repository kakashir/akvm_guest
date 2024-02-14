.balign 4096
.section ".data16","aw"
	.fill 2048, 1, 0
stack_top:
	/*
	  each gdt item has 8 bytes by 3 entired
	  (null, code, data) = 24 bytes
	*/
	.balign	8
gdt:
	.4byte 0x00000000 /* null selector */
	.4byte 0x00000000
	.4byte 0x0000ffff /* 32 bit code */
	.4byte 0x00cf9a00
	.4byte 0x0000ffff /* 32 bit data*/
	.4byte 0x00cf9200
gdt_tab_desc:
	.2byte gdt_tab_desc - gdt
	.4byte gdt
.balign 4096
.section ".text16","ax"
.code16

.global start16
start16:
	mov $stack_top, %ax
	mov %ax, %sp
	mov %ax, %bp
	mov $0, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %ss
	mov %ax, %fs
	mov %ax, %gs
	push %ax
	push $__start16
	retf

__start16:
	/* switch to protect mode */
	lgdt gdt_tab_desc
	mov $0x31, %eax
	mov %eax, %cr0

	/* flush segment cache */
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %ss
	mov %ax, %fs
	mov %ax, %gs
	pushl $0x8
	pushl $start32
	retfl
