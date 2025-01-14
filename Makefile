INCLUDES=-Iinclude
CFLAGS=$(INCLUDES) -ggdb

all: bin/dtb

bin/dtb: include/dtb/dtb.h src/dtb.c
	gcc src/dtb.c -o bin/dtb $(CFLAGS)
