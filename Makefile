asm_object = start16.o start32.o start64.o
c_object = main.o x86.o
h_object = config.h x86.h vm_service.h types.h entry.h
linker = linker.lds.S
flags = -O0 -g -no-pie -fno-pic -mcmodel=kernel -nostartfiles \
-nodefaultlibs -nolibc -nostdlib -nostdlib++ -fno-stack-protector \
-mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -mno-80387 -mno-fp-ret-in-387
cc_flags = $(flags)
asm_cc_flags = $(flags) -D__ASM__

binary: $(asm_object) $(c_object) $(linker)
	gcc -E -P -x c $(linker) > $(linker).out
	gcc $(flags) -T $(linker).out $(asm_object) $(c_object)

$(asm_object): %.o: %.S $(h_object)
	gcc $(asm_cc_flags) -c "$<" -o "$@"

$(c_object): %.o: %.c $(h_object)
	gcc $(cc_flags) -c "$<" -o "$@"

.PHONY clean:
	rm -f *.o
	rm -f binary
