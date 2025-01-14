#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include "dtb/dtb.h"

#define DTB_MAGIC_LE DTB_BYTESWAP32(0xd00dfeed)

dtb *dtb_fromptr(void *ptr)
{
    dtb *devicetree = (dtb *)ptr;

    if (devicetree == NULL) {
        return NULL;
    }

    if (devicetree->magic != DTB_MAGIC_LE) {
        return NULL;
    }

    return devicetree;
}

int main(int argc, char **argv)
{
    int fd = open(argv[1], O_RDONLY);
    assert(fd != -1);

    struct stat buf;
    assert(fstat(fd, &buf) == 0);

    void *ptr = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    dtb *devicetree = dtb_fromptr(ptr);
    if (devicetree == NULL) {
        printf("Malformed device tree\n");
        return 0;
    }
}
