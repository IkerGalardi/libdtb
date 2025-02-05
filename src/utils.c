#include "utils.h"
#include "debug.h"

int strcmp_nodename(const char *pathpart, const char *nodename)
{
    while (!(*pathpart == '\0' || *nodename == '\0')) {
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

dtb_u32 *next_token(dtb_u32 *token)
{
    assert((dtb_u64)token % 4 == 0);

    if (*token == DTB_BEGIN_NODE) {
        DEBUG_PRINT("%p: DTB_BEGIN_NODE %s\n", (void *)token, (char *)(token + 1));
        token++;

        char *tokenchar = (char *)token;
        while (*tokenchar != '\0') {
            tokenchar++;
        }

        if ((dtb_u64)tokenchar % 4 != 0) {
            tokenchar += 4 - (dtb_u64)tokenchar % 4;
        }
        token = (dtb_u32 *)tokenchar;

        while (*token == 0) {
            token++;
        }

        assert((dtb_u64)token % 4 == 0);
        assert(*token != DTB_END);
    } else if (*token == DTB_END_NODE) {
        DEBUG_PRINT("%p: DTB_END_NODE\n", (void *)token);
        token++;

        assert(*token != DTB_PROP);
    } else if (*token == DTB_PROP) {
        dtb_u32 len = DTB_BYTESWAP32(*(token + 1));
        DEBUG_PRINT("%p: PROP %" PRIu32 "\n", (void *)token, len);
        if (len % sizeof(dtb_u32) == 0) {
            token += len / sizeof(dtb_u32) + 3;
        } else {
            dtb_u32 len_rounding = 4 - (len % 4);
            token += (len + len_rounding) / sizeof(dtb_u32) + 3;
        }

        assert(*token != DTB_END);
    } else if (*token == DTB_NOP) {
        DEBUG_PRINT("%p: DTB_NOP %s\n", (void *)token, (char *)(token + 1));
        token++;
    } else {
        assert(false);
    }

    assert((dtb_u64)token % 4 == 0);
    return token;
}
