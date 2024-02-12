asm_object = start16.o start32.o start64.o
c_object = main.o
linker = linker.lds.S
cc_flags = -O0 -g

binary: $(asm_object) $(c_object) $(linker)
	ld -T $(linker) $(asm_object) $(c_object)

$(asm_object): %.o: %.S
	as -c "$<" -o "$@"

$(c_object): %.o: %.c
	gcc $(cc_flags) -c "$<" -o "$@"

.PHONY clean:
	rm -f *.o
	rm -f binary
