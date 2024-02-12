.section ".data32","aw"

long_mode_page_table:
	.fill 4096, 3, 0

.section ".text32","ax"
.code32

.global start32
	movl $0x1234, %eax
