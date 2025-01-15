#ifndef _TEST_H
#define _TEST_H

#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "dtb/dtb.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void *map_device_tree(const char *path)
{
    int fd = open(path, O_RDONLY);
    assert(fd != -1);

    struct stat buf;
    assert(fstat(fd, &buf) == 0);

    void *ptr = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    assert(ptr != MAP_FAILED);

    return ptr;
}

#endif // _TEST_H
