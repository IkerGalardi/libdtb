#include "dtb/dtb.h"
#include "utils.h"
#include "debug.h"

#define DTB_MAGIC_LE DTB_BYTESWAP32(0xd00dfeed)

extern int strcmp(const char *, const char *);

dtb *dtb_fromptr(void *ptr)
{
    dtb *devicetree = (dtb *)ptr;

    if (devicetree == DTB_NULL) {
        return DTB_NULL;
    }

    if (devicetree->magic != DTB_MAGIC_LE) {
        return DTB_NULL;
    }

    if (devicetree->last_comp_version != DTB_BYTESWAP32(16)) {
        return DTB_NULL;
    }

    return devicetree;
}

dtb_node dtb_find(dtb *devicetree, const char *path)
{
    dtb_u32 *struct_block = (dtb_u32 *)((uint8_t *)devicetree + DTB_BYTESWAP32(devicetree->off_dt_struct));

    // The first block from the struct node should be a DTB_BEGIN_NODE as it should be refering to
    // the root node of the device tree. If that is not the case we return null to signal an error.
    if (*struct_block != DTB_BEGIN_NODE) {
        return DTB_NULL;
    }

    if (strcmp(path, "/") == 0) {
        return (dtb_node)struct_block;
    }

    // Always needs to be an absolute path.
    if (path[0] != '/') {
        DEBUG_PRINT("Path does not start with /\n");
        return DTB_NULL;
    }

    char parsed_path[10][256] = {0};
    size_t parsed_i = 0;
    size_t path_depth = 0;
    int i = 1;
    while (path[i] != '\0') {
        if (path[i] == '/') {
            path_depth++;
            parsed_i = 0;
        } else {
            parsed_path[path_depth][parsed_i] = path[i];
            parsed_i++;
        }

        i++;
    }

    dtb_u32 *token = struct_block + 1;
    dtb_u32 parsing_depth = 0;
    while (*token != DTB_END) {
        if (*token == DTB_BEGIN_NODE) {
            token++;
            DEBUG_PRINT("BEGIN NODE %s, comparing with %s\n", (char *)token, parsed_path[parsing_depth]);
            if (strcmp_nodename(parsed_path[parsing_depth], (char *)token) == 0) {
                if (parsing_depth == path_depth) {
                    return (dtb_node)token - 1;
                }

                parsing_depth++;
            } else {
                token = dtb_next_sibling(token - 1);
                if (token == DTB_NULL) {
                    return DTB_NULL;
                }
                token--;
            }
        } else if (*token == DTB_PROP) {
            dtb_u32 len = DTB_BYTESWAP32(*(token + 1));
            token += len / sizeof(dtb_u32) + 2;
            DEBUG_PRINT("PROP %" PRIu32 "\n", len);
        } else if (*token == DTB_NOP) {
            DEBUG_PRINT("NOP\n");
        } else if (*token == DTB_END_NODE) {
            DEBUG_PRINT("DTB_END_NODE\n");
        }

        token++;
    }

    return DTB_NULL;
}

dtb_node dtb_find_next(dtb_node node, char *name)
{
    node = dtb_next_sibling(node);

    while (node != DTB_NULL) {
        char *nodename = dtb_node_name(node);

        if (strcmp_nodename(nodename, name) == 0) {
            return node;
        }

        node = dtb_next_sibling(node);
    }

    return DTB_NULL;
}

static dtb_node find_by_phandle_from(dtb *devicetree, dtb_node from, dtb_u32 phandle)
{
    dtb_foreach_child(from, child) {
        dtb_foreach_property(child, prop) {
            char *propname = dtb_property_name(devicetree, prop);
            if (strcmp_nodename("phandle", propname) == 0 && dtb_property_uint32(prop) == phandle) {
                return child;
            }
        }

        dtb_node node = find_by_phandle_from(devicetree, child, phandle);
        if (node != DTB_NULL) {
            return node;
        }
    }

    return DTB_NULL;
}

dtb_node dtb_find_by_phandle(dtb *devicetree, dtb_u32 phandle)
{
    dtb_node root_node = dtb_find(devicetree, "/");
    assert(root_node != DTB_NULL);

    return find_by_phandle_from(devicetree, root_node, phandle);
}

char *dtb_property_name(dtb *devicetree, dtb_node node)
{
    char *strings = (char *)devicetree + DTB_BYTESWAP32(devicetree->off_dt_strings);
    dtb_u32 stroff = DTB_BYTESWAP32(*(node + 2));
    return strings + stroff;
}

dtb_u32 dtb_property_uint32(dtb_property prop)
{
    return DTB_BYTESWAP32(*(prop + 3));
}

uint64_t dtb_property_uint64(dtb_property prop)
{
    return DTB_BYTESWAP64(*(uint64_t *)(prop + 3));
}

char *dtb_property_string(dtb_property prop)
{
    return (char *)(prop + 3);
}

char *dtb_property_array(dtb_property prop)
{
    return (void *)(prop + 3);
}

dtb_property dtb_first_property(dtb_node node)
{
    dtb_u32 *token = next_token(node);
    while (*token != DTB_PROP) {
        if (*token == DTB_END || *token == DTB_BEGIN_NODE || *token == DTB_END_NODE) {
            return DTB_NULL;
        }

        token = next_token(token);
    }

    return token;
}

dtb_property dtb_next_property(dtb_property prop)
{
    dtb_u32 *token = next_token(prop);
    while (*token != DTB_PROP) {
        if (*token == DTB_END || *token == DTB_BEGIN_NODE || *token == DTB_END_NODE) {
            return DTB_NULL;
        }

        token = next_token(token);
    }

    return token;
}

dtb_node dtb_first_child(dtb_node node)
{
    assert(*node == DTB_BEGIN_NODE);

    dtb_u32 *token = next_token(node);
    while (*token != DTB_BEGIN_NODE) {
        if (*token == DTB_END || *token == DTB_END_NODE) {
            return DTB_NULL;
        }

        token = next_token(token);
    }

    assert(*token == DTB_BEGIN_NODE);
    return token;
}

dtb_node dtb_next_sibling(dtb_node node)
{
    int depth = 0;
    while (*node != DTB_END) {
        if (*node == DTB_BEGIN_NODE) {
            node++;
            depth++;
            DEBUG_PRINT("  BEGIN_NODE %s, new depth = %d\n", (char *)node, depth);
        } else if (*node == DTB_PROP) {
            dtb_u32 len = DTB_BYTESWAP32(*(node + 1));
            int len_rounding = 4 - len % 4;
            if (len % 4 == 0) {
                node += len / sizeof(dtb_u32) + 2;
            } else {
                node += (len + len_rounding) / sizeof(dtb_u32) + 2;
            }
            DEBUG_PRINT("  PROP %" PRIu32 "\n", len);
        } else if (*node == DTB_NOP) {
            DEBUG_PRINT("  NOP\n");
        } else if (*node == DTB_END_NODE) {
            depth--;
            DEBUG_PRINT("  END NODE, new depth = %d\n", depth);
            if (depth <= 0) {
                node++;
                break;
            }
        }
        node++;
    }

    while (*node != DTB_BEGIN_NODE) {
        if (*node == DTB_END) {
            break;
        }

        if (*node == DTB_END_NODE) {
            return DTB_NULL;
        }

        node++;
    }

    return node;
}

dtb_rsvmap_entry *dtb_first_rsvmap_entry(dtb *devicetree)
{
    uint8_t *dtb_ptr = (uint8_t *)devicetree;
    dtb_rsvmap_entry *entry = (dtb_rsvmap_entry *)(dtb_ptr + DTB_BYTESWAP32(devicetree->off_mem_rsvmap));

    if (entry->address == 0 && entry->size == 0) {
        return DTB_NULL;
    }

    return entry;
}

dtb_rsvmap_entry *dtb_next_rsvmap_entry(dtb_rsvmap_entry *entry)
{
    entry++;

    if (entry->address == 0 && entry->size == 0) {
        return DTB_NULL;
    }

    return entry;
}
