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

#define MAX_LINE_WIDTH 70

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

void print_test_result(const char *name, bool succeded)
{
    char buff[MAX_LINE_WIDTH +  5] = {0};
    memset(buff, ' ', MAX_LINE_WIDTH-2);
    int i = 0;
    while (name[i] != '\0') {
        buff[i] = name[i];
        i++;
    }

    #define ANSI_COLOR_RED     "\x1b[31m"
    if (succeded == true) {
        buff[MAX_LINE_WIDTH - 2]  = 'S';
        buff[MAX_LINE_WIDTH - 3]  = 'S';
        buff[MAX_LINE_WIDTH - 4]  = 'E';
        buff[MAX_LINE_WIDTH - 5]  = 'C';
        buff[MAX_LINE_WIDTH - 6]  = 'C';
        buff[MAX_LINE_WIDTH - 7]  = 'U';
        buff[MAX_LINE_WIDTH - 8]  = 'S';
        buff[MAX_LINE_WIDTH - 9]  = 'm';
        buff[MAX_LINE_WIDTH - 10] = '2';
        buff[MAX_LINE_WIDTH - 11] = '3';
        buff[MAX_LINE_WIDTH - 12] = '[';
        buff[MAX_LINE_WIDTH - 13] = '\x1b';
    } else {
        buff[MAX_LINE_WIDTH - 2] = 'D';
        buff[MAX_LINE_WIDTH - 3] = 'E';
        buff[MAX_LINE_WIDTH - 4] = 'L';
        buff[MAX_LINE_WIDTH - 5] = 'I';
        buff[MAX_LINE_WIDTH - 6] = 'A';
        buff[MAX_LINE_WIDTH - 7] = 'F';
        buff[MAX_LINE_WIDTH - 9]  = 'm';
        buff[MAX_LINE_WIDTH - 10] = '1';
        buff[MAX_LINE_WIDTH - 11] = '3';
        buff[MAX_LINE_WIDTH - 12] = '[';
        buff[MAX_LINE_WIDTH - 13] = '\x1b';
    }

    printf("%s" "\x1b[0m\n", buff);

    if (succeded == false) {
        abort();
    }
}

#endif // _TEST_H
