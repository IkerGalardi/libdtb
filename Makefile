INCLUDES=-Iinclude
CFLAGS=$(INCLUDES) -Wall -Wextra -Wpedantic

all: libdtb.a libdtb-debug.a

libdtb.a: bin/dtb.o bin/utils.o
	ar rcs libdtb.a bin/dtb.o

libdtb-debug.a: bin/dtb-debug.o bin/utils-debug.o
	ar rcs libdtb-debug.a bin/dtb-debug.o

bin/dtb.o: include/dtb/dtb.h src/dtb.c src/utils.h
	$(CC) -c src/dtb.c -o bin/dtb.o -O2 $(CFLAGS)

bin/utils.o: src/utils.h src/utils.c
	$(CC) -c src/utils.c -o bin/utils.o -O2 $(CFLAGS)

bin/dtb-debug.o: include/dtb/dtb.h src/dtb.c src/utils.h
	$(CC) -c src/dtb.c -o bin/dtb-debug.o -ggdb $(CFLAGS)

bin/utils-debug.o: src/utils.h src/utils.c
	$(CC) -c src/utils.c -o bin/utils-debug.o -ggdb $(CFLAGS)


bin/test_qemu-virt: bin/dtb-debug.o bin/utils-debug.o test/test_qemu-virt.c test/test.h include/dtb/dtb.h
	$(CC) -c test/test_qemu-virt.c -o bin/test_qemu-virt.o $(CFLAGS) -ggdb
	$(CC) -o bin/test_qemu-virt bin/dtb-debug.o bin/utils-debug.o bin/test_qemu-virt.o -ggdb

test: bin/test_qemu-virt
	@ bin/test_qemu-virt dtbfiles/qemu-virt.dtb
