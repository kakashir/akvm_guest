asm_object = start16.o start32.o start64.o
linker = linker.lds.S

binary: $(asm_object) $(linker)
	ld -T $(linker) $(asm_object)

$(asm_object): %.o: %.S
	as -c "$<" -o "$@"

.PHONY clean:
	rm -f *.o
	rm -f binary
