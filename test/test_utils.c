#include "test.h"
#include "utils.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    if (strcmp_nodename("memory", "memory@800000") == 0) {
        printf("utils: strcmp_nodename 'memory' == 'memory@800000' " ANSI_COLOR_GREEN "SUCCESS" ANSI_COLOR_RESET "\n");
    } else {
        printf("utils: strcmp_nodename 'memory' == 'memory@800000' " ANSI_COLOR_RED "FAILED" ANSI_COLOR_RESET "\n");
    }

    if (strcmp_nodename("soc", "soc") == 0) {
        printf("utils: strcmp_nodename 'soc' == 'soc' " ANSI_COLOR_GREEN "SUCCESS" ANSI_COLOR_RESET "\n");
    } else {
        printf("utils: strcmp_nodename 'soc' == 'soc' " ANSI_COLOR_RED "FAILED" ANSI_COLOR_RESET "\n");
    }

    if (strcmp_nodename("memory", "soc") != 0) {
        printf("utils: strcmp_nodename 'memory' != 'soc' " ANSI_COLOR_GREEN "SUCCESS" ANSI_COLOR_RESET "\n");
    } else {
        printf("utils: strcmp_nodename 'memory' != 'soc' " ANSI_COLOR_RED "FAILED" ANSI_COLOR_RESET "\n");
    }
}
