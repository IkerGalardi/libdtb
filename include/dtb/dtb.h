#ifndef _DTB_H
#define _DTB_H

#include <stdint.h>

#define DTB_BYTESWAP32(num) ((((num)>>24)&0xff) | (((num)<<8)&0xff0000) | \
                        (((num)>>8)&0xff00) | (((num)<<24)&0xff000000))

#define DTB_BYTESWAP64(num) (BYTESWAP32(num) << 32 | BYTESWAP32((num) >> 32))

#define DTB_CONCAT_HELPER(a, b) a ## b
#define DTB_CONCAT(a, b) DTB_CONCAT_HELPER(a, b)

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

/**
 * @brief Create a device tree object from a pointer.
 *
 * @param ptr: pointer to a buffer containing the device tree.
 */
dtb *dtb_fromptr(void *ptr);

dtb_node dtb_find(dtb *devicetree, const char *path);

#define dtb_property_name(node) (char *)((uint32_t *)node+1)

#define dtb_foreach_property(dtb, node, x) { \
    char *__strings = (char *)dtb + DTB_BYTESWAP32(dtb->off_dt_strings); \
    uint32_t *__prop = node + 1; \
    while (1) { \
        if (*__prop == DTB_NOP) { \
            __prop++;\
        } else if (*__prop == DTB_PROP) {\
            uint32_t proplen = DTB_BYTESWAP32(*(__prop + 1));\
            uint32_t __stroff = DTB_BYTESWAP32(*(__prop + 2));\
            char *propname = __strings + __stroff; \
            uint32_t __attribute__((unused)) *prop = __prop + 3; \
            x \
            __prop += proplen / sizeof(uint32_t) + 2; \
        } else if (*__prop == DTB_END || *__prop == DTB_BEGIN_NODE || *__prop == DTB_END_NODE) { \
            break;\
        }\
        __prop++; \
    } \
}

#define dtb_foreach_rsvmap_entry(dtb, x) {                                      \
        uint8_t *__dtb_ptr = (uint8_t *)dtb;                                    \
        uint8_t *__entry_ptr = __dtb_ptr + DTB_BYTESWAP32(dtb->off_mem_rsvmap); \
        dtb_rsvmap_entry *entry = (dtb_rsvmap_entry *)__entry_ptr;              \
        while (!(entry->address == 0 && entry->size == 0)) {                    \
            x                                                                   \
        }                                                                       \
    }

#endif // _DTB_H
