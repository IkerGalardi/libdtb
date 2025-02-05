#ifndef _UTILS_
#define _UTILS_

#include "dtb/dtb.h"

int strcmp_nodename(const char *pathpart, const char *nodename);

dtb_u32 *next_token(dtb_u32 *token);

#endif // _UTILS_
