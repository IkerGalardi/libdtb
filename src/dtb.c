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

#define DTB_BEGIN_NODE DTB_BYTESWAP32((uint32_t)0x1)
#define DTB_END_NODE   DTB_BYTESWAP32((uint32_t)0x2)
#define DTB_PROP       DTB_BYTESWAP32((uint32_t)0x3)
#define DTB_NOP        DTB_BYTESWAP32((uint32_t)0x4)
#define DTB_END        DTB_BYTESWAP32((uint32_t)0x9)

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

dtb_node dtb_find(dtb *devicetree, const char *path)
{
    uint32_t *struct_block = (uint8_t *)devicetree + DTB_BYTESWAP32(devicetree->off_dt_struct);

    // The first block from the struct node should be a DTB_BEGIN_NODE as it should be refering to
    // the root node of the device tree. If that is not the case we return null to signal an error.
    if (*struct_block != DTB_BEGIN_NODE) {
        return NULL;
    }

    if (strcmp(path, "/") == 0) {
        return (dtb_node)struct_block;
    }

    // TODO: actually implement the searching of the node
    assert(0);
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

    dtb_foreach_rsvmap_entry(devicetree, {
        printf("Reserved memory at %lx of size %lx\n", entry->address, entry->size);
    });
}
