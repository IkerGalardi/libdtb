#ifndef _DEBUG_H
#define _DEBUG_H

#define DEBUG_CONFIG_PRINTS  0
#define DEBUG_CONFIG_ASSERTS 0

#if DEBUG_CONFIG_PRINTS
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <inttypes.h>

#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

#if DEBUG_CONFIG_ASSERTS
#include <assert.h>
#include <stdbool.h>
#else
#define assert(x)
#endif

#endif // _DEBUG_H
