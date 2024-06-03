SUB_DIR = $(shell pwd)
ROOT_DIR = $(SUB_DIR)/..
c_object = b_allocator.o mm.o paging.o
h_object += $(shell find $(SUB_DIR) -name "*.h" 2>/dev/null)
h_object += $(ROOT_DIR)/include/*.h

BUILD: $(c_object)

$(c_object): %.o: %.c $(h_object)
	$(cc) $(cc_flags) -c "$<" -o "$@" -I$(ROOT_DIR)
