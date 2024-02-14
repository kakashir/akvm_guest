.balign 4096
.section ".text64_low","ax"
.code64

.global start64_low
start64_low:
	lea start64(%rip), %rcx
	call *%rcx

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
1:
	lea start_kernel(%rip), %rcx
	call *%rcx
