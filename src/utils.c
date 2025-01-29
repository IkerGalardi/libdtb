#include "utils.h"
#include <stddef.h>
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

uint32_t *next_token(uint32_t *token)
{
    assert((uint64_t)token % 4 == 0);

    if (*token == DTB_BEGIN_NODE) {
        DEBUG_PRINT("%p: DTB_BEGIN_NODE %s\n", (void *)token, (char *)(token + 1));
        token++;

        char *tokenchar = (char *)token;
        while (*tokenchar != '\0') {
            tokenchar++;
        }

        tokenchar += (uint64_t)tokenchar % 4;
        token = (uint32_t *)tokenchar;
        token++;

        assert(*token != DTB_END);
    } else if (*token == DTB_END_NODE) {
        DEBUG_PRINT("%p: DTB_END_NODE\n", (void *)token);
        token++;

        assert(*token != DTB_PROP);
    } else if (*token == DTB_PROP) {
        uint32_t len = DTB_BYTESWAP32(*(token + 1));
        DEBUG_PRINT("%p: PROP %" PRIu32 "\n", (void *)token, len);
        if (len % sizeof(uint32_t) == 0) {
            token += len / sizeof(uint32_t) + 3;
        } else {
            uint32_t len_rounding = 4 - (len % 4);
            token += (len + len_rounding) / sizeof(uint32_t) + 3;
        }

        assert(*token != DTB_END);
    } else if (*token == DTB_NOP) {
        DEBUG_PRINT("%p: DTB_NOP %s\n", (void *)token, (char *)(token + 1));
        token++;
    } else {
        assert(false);
    }

    assert((uint64_t)token % 4 == 0);
    return token;
}
