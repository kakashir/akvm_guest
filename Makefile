ROOT_DIR = $(shell pwd)

c_object = main.o io.o
h_object = $(shell find $(ROOT_DIR) -maxdepth 1 -name "*.h" 2>/dev/null)
h_object += $(shell find $(ROOT_DIR)/include -name "*.h" 2>/dev/null)
linker = linker.lds.S

sub_dir = lib mm cpu
clean_sub_dir = $(sub_dir)
sub_c_object = $(shell find $(ROOT_DIR)/lib -name "*.o" 2>/dev/null)
sub_c_object += $(shell find $(ROOT_DIR)/mm -name "*.o" 2>/dev/null)
sub_c_object += $(shell find $(ROOT_DIR)/cpu -name "*.o" 2>/dev/null)

cc = gcc
flags = -O0 -g -no-pie -fno-pic -mcmodel=kernel -nostartfiles \
-nodefaultlibs -nolibc -nostdlib -nostdlib++ -fno-stack-protector \
-mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -mno-80387 -mno-fp-ret-in-387 \
-Wno-builtin-declaration-mismatch \
-I$(ROOT_DIR)/include


cc_flags = $(flags)
asm_cc_flags = $(flags) -D__ASM__

binary: $(c_object) SUBDIR $(linker).out
	$(CC) $(flags) -T $(linker).out $(c_object) $(sub_c_object)

$(c_object): %.o: %.c $(h_object)
	$(cc) $(cc_flags) -c "$<" -o "$@"

$(linker).out: $(linker) $(h_object)
	$(cc) -I$(ROOT_DIR)/include -E -P -x c $(linker) > $(linker).out

.PHONY SUBDIR: $(sub_dir)
$(sub_dir):
	cc="$(cc)" \
	cc_flags="$(cc_flags)" \
	asm_cc_flags="$(asm_cc_flags)" \
	h_object="$(h_object)" \
	make -C $(ROOT_DIR)/"$@"

.PHONY clean:
	rm -f *.o
	rm -f $(sub_c_object)
	rm -f binary
