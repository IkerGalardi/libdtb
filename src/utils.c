#include "utils.h"
#include "debug.h"
#include "dtb/dtb.h"

int strcmp_nodename(const char *pathpart, const char *nodename)
{
    while (!(*pathpart == '\0' || *nodename == '\0' || *nodename == '@')) {
        if (*pathpart != *nodename) {
            break;
        }

        pathpart++;
        nodename++;
    }

    if (*pathpart == '\0' && *nodename == '\0') {
        return 0;
    }

    if (*pathpart == '\0' && *nodename == '@') {
        return 0;
    }

    return 1;
}

uint32_t *skip_until_same_depth(uint32_t *token)
{
    DEBUG_PRINT("SKIPPING %s\n", (char *)(token));
    int depth = 0;
    while (*token != DTB_END) {
        if (*token == DTB_BEGIN_NODE) {
            token++;
            depth++;
            DEBUG_PRINT("  BEGIN_NODE %s, new depth = %d\n", (char *)token, depth);
        } else if (*token == DTB_PROP) {
            uint32_t len = DTB_BYTESWAP32(*(token + 1));
            token += len / sizeof(uint32_t) + 2;
            DEBUG_PRINT("  PROP %" PRIu32 "\n", len);
        } else if (*token == DTB_NOP) {
            DEBUG_PRINT("  NOP\n");
        } else if (*token == DTB_END_NODE) {
            depth--;
            DEBUG_PRINT("  END NODE, new depth = %d\n", depth);
            if (depth <= 0) {
                break;
            }
        }
        token++;
    }

    while (*token == DTB_NOP) {
        token++;

        if (*token == DTB_END) {
            break;
        }
    }

    return token;
}
