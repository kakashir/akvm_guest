SUB_DIR = $(shell pwd)
ROOT_DIR= $(SUB_DIR)/..
c_object = debug.o print.o bitops.o linker.o
h_object = $(ROOT_DIR)/include/lib/*.h

BUILD: $(c_object)

$(c_object): %.o: %.c $(h_object)
	$(cc) $(cc_flags) -c "$<" -o "$@" -I$(ROOT_DIR)
