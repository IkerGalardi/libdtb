#ifndef _UTILS_
#define _UTILS_

#include <stdint.h>

int strcmp_nodename(const char *pathpart, const char *nodename);

uint32_t *skip_until_same_depth(uint32_t *token);

#endif // _UTILS_
