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
    return NULL;
}
