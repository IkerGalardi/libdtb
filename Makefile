INCLUDES=-Iinclude
CFLAGS=$(INCLUDES) -Wall -Wextra -Wpedantic

all: libdtb.a libdtb-debug.a

libdtb.a: bin/dtb.o
	ar rcs libdtb.a bin/dtb.o

libdtb-debug.a: bin/dtb-debug.o
	ar rcs libdtb-debug.a bin/dtb-debug.o

bin/dtb.o: include/dtb/dtb.h src/dtb.c
	$(CC) -c src/dtb.c -o bin/dtb.o -O2 $(CFLAGS)

bin/dtb-debug.o: include/dtb/dtb.h src/dtb.c
	$(CC) -c src/dtb.c -o bin/dtb-debug.o -ggdb $(CFLAGS)

bin/test_qemu-virt: libdtb-debug.a test/test_qemu-virt.c test/test.h
	$(CC) test/test_qemu-virt.c -o bin/test_qemu-virt $(CFLAGS) -ggdb -L. -ldtb

test: bin/test_qemu-virt
	@ bin/test_qemu-virt dtbfiles/qemu-virt.dtb
