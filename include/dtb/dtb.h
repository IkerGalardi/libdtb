#ifndef _DTB_H
#define _DTB_H

#include <stdint.h>

#define DTB_BYTESWAP32(num) ((((num)>>24)&0xff) | (((num)<<8)&0xff0000) | \
                        (((num)>>8)&0xff00) | (((num)<<24)&0xff000000))

#define DTB_BYTESWAP64(num) (BYTESWAP32(num) << 32 | BYTESWAP32((num) >> 32))

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

/**
 * @brief Create a device tree object from a pointer.
 *
 * @param ptr: pointer to a buffer containing the device tree.
 */
dtb *dtb_fromptr(void *ptr);

#define dtb_foreach_rsvmap_entry(dtb, x) {                                      \
        uint8_t *__dtb_ptr = (uint8_t *)dtb;                                    \
        uint8_t *__entry_ptr = __dtb_ptr + DTB_BYTESWAP32(dtb->off_mem_rsvmap); \
        dtb_rsvmap_entry *entry = (dtb_rsvmap_entry *)__entry_ptr;              \
        while (!(entry->address == 0 && entry->size == 0)) {                    \
            x                                                                   \
        }                                                                       \
    }

#endif // _DTB_H
