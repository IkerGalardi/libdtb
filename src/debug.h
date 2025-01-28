#ifndef _DEBUG_H
#define _DEBUG_H

#if 0
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <assert.h>

#define DEBUG_PRINT(...) printf(__VA_ARGS__)

#else
#define DEBUG_PRINT(...)
#define assert(x)
#endif

#endif // _DEBUG_H
