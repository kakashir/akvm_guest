SUB_DIR = $(shell pwd)
ROOT_DIR= $(SUB_DIR)/..
c_object = cpu.o x86.o
h_object += $(ROOT_DIR)/include/lib/*.h
h_object += $(shell find $(SUB_DIR) -name "*.h" 2>/dev/null)

asm_object = start.o start32.o start64.o entry.o

BUILD: $(c_object) $(asm_object)

$(c_object): %.o: %.c $(h_object)
	$(cc) $(cc_flags) -c "$<" -o "$@" -I$(ROOT_DIR)

$(asm_object): %.o: %.S $(h_object)
	$(cc) $(asm_cc_flags) -c "$<" -o "$@"
