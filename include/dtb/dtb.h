#ifndef _DTB_H
#define _DTB_H

#include <stdint.h>

#define DTB_BYTESWAP32(num) ((((num)>>24)&0xff) | (((num)<<8)&0xff0000) | \
                        (((num)>>8)&0xff00) | (((num)<<24)&0xff000000))

#define DTB_BYTESWAP64(num) (DTB_BYTESWAP32(num) << 32 | DTB_BYTESWAP32((num) >> 32))

#define DTB_BEGIN_NODE DTB_BYTESWAP32((uint32_t)0x1)
#define DTB_END_NODE   DTB_BYTESWAP32((uint32_t)0x2)
#define DTB_PROP       DTB_BYTESWAP32((uint32_t)0x3)
#define DTB_NOP        DTB_BYTESWAP32((uint32_t)0x4)
#define DTB_END        DTB_BYTESWAP32((uint32_t)0x9)

/**
 * @brief Device tree handle
 */
typedef struct
{
    uint32_t magic;
    uint32_t totalsize;
    uint32_t off_dt_struct;
    uint32_t off_dt_strings;
    uint32_t off_mem_rsvmap;
    uint32_t version;
    uint32_t last_comp_version;
    uint32_t boot_cpuid_phys;
    uint32_t size_dt_strings;
    uint32_t size_dt_struct;
} dtb;

typedef struct __attribute__((packed))
{
    uint64_t address;
    uint64_t size;
} dtb_rsvmap_entry;

typedef uint32_t* dtb_node;
typedef uint32_t* dtb_property;

/**
 * @brief Create a device tree object from a pointer.
 *
 * @param ptr: pointer to a buffer containing the device tree.
 */
dtb *dtb_fromptr(void *ptr);

dtb_node dtb_find(dtb *devicetree, const char *path);

#define dtb_node_name(node) (char *)((uint32_t *)node+1)

char *dtb_property_name(dtb *devicetree, dtb_node node);

uint32_t dtb_property_uint32(dtb_property prop);

uint64_t dtb_property_uint64(dtb_property prop);

char *dtb_property_string(dtb_property prop);

char *dtb_property_array(dtb_property prop);

#define dtb_foreach_property(node, name) for (dtb_property name = dtb_first_property(node); name != NULL; name = dtb_next_property(name))

dtb_property dtb_first_property(dtb_property prop);

dtb_property dtb_next_property(dtb_property prop);

dtb_node dtb_first_child(dtb_node node);

dtb_node dtb_next_sibling(dtb_node node);

#define dtb_foreach_child(node, name) for (dtb_node name = dtb_first_child(node); name != NULL; name = dtb_next_sibling(name))

dtb_rsvmap_entry *dtb_first_rsvmap_entry(dtb *devicetree);

dtb_rsvmap_entry *dtb_next_rsvmap_entry(dtb_rsvmap_entry *entry);

#define dtb_foreach_rsvmap_entry(dtb, entry) for (dtb_rsvmap_entry *entry = dtb_first_rsvmap_entry(dtb); entry != NULL; entry = dtb_next_rsvmap_entry(entry))

#endif // _DTB_H
