asm_object = start16.o start32.o start64.o
c_object = main.o
h_object = config.h
linker = linker.lds.S
cc_flags = -O0 -g -no-pie -fno-pic -mcmodel=kernel -nostartfiles -nodefaultlibs -nolibc -nostdlib -nostdlib++

binary: $(asm_object) $(c_object) $(linker)
	gcc -E -P -x c $(linker) > $(linker).out
	gcc $(cc_flags) -T $(linker).out $(asm_object) $(c_object)

$(asm_object): %.o: %.S $(h_object)
	gcc $(cc_flags) -c "$<" -o "$@"

$(c_object): %.o: %.c $(h_object)
	gcc $(cc_flags) -c "$<" -o "$@"

.PHONY clean:
	rm -f *.o
	rm -f binary
