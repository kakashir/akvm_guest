.section ".text64","ax"
.code64

.global start64
	mov $0x12345678, %rax
	jmp start_kernel
.section ".data64","aw"
	.fill 4096,2,0

.section ".bss64","aw", @nobits
	.byte 0

.section ".bss","aw", @nobits
