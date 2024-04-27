ROOT_DIR = $(shell pwd)

asm_object = start16.o start32.o start64.o
c_object = main.o x86.o io.o
h_object = *.h
linker = linker.lds.S

sub_dir = lib
clean_sub_dir = $(sub_dir)
sub_c_object = $(shell find $(ROOT_DIR)/lib -name "*.o" 2>/dev/null)

cc = gcc
flags = -O0 -g -no-pie -fno-pic -mcmodel=kernel -nostartfiles \
-nodefaultlibs -nolibc -nostdlib -nostdlib++ -fno-stack-protector \
-mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -mno-80387 -mno-fp-ret-in-387

cc_flags = $(flags)
asm_cc_flags = $(flags) -D__ASM__

binary: $(asm_object) $(c_object) SUBDIR $(linker).out
	$(CC) $(flags) -T $(linker).out $(asm_object) $(c_object) $(sub_c_object)

$(asm_object): %.o: %.S $(h_object)
	$(cc) $(asm_cc_flags) -c "$<" -o "$@"

$(c_object): %.o: %.c $(h_object)
	$(cc) $(cc_flags) -c "$<" -o "$@"

$(linker).out: $(linker)
	$(cc) -E -P -x c $(linker) > $(linker).out

.PHONY SUBDIR: $(sub_dir)
$(sub_dir):
	cc="$(cc)" \
	cc_flags="$(cc_flags)" \
	asm_cc_flags="$(asm_cc_flags)" \
	make -C $(ROOT_DIR)/"$@"

.PHONY clean:
	rm -f *.o
	rm -f $(sub_c_object)
	rm -f binary
