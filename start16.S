.section ".data16","aw"
	.fill 4096, 1, 0
stack_top:

protect_mode_page_table:
	.fill 4096, 1, 0

.section ".text16","ax"
.code16

.global start16
start16:
	movl $0, %eax
