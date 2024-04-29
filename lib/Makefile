SUB_DIR = $(shell pwd)
ROOT_DIR= $(SUB_DIR)/..
c_object = debug.o print.o
h_object = *.h

BUILD: $(c_object)

$(c_object): %.o: %.c $(h_object)
	$(cc) $(cc_flags) -c "$<" -o "$@" -I$(ROOT_DIR)
