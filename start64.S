.balign 4096
.section ".data64","aw"

.balign 1
.section ".bss64","aw", @nobits
.section ".bss","aw", @nobits

.balign 8
.section ".text64","ax"
.code64

.global start64
start64:
	mov $0x12345678, %rax
	jmp start_kernel
