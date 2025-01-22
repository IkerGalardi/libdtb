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

    uint32_t tokenlist[] = {
        DTB_BEGIN_NODE, 0,  // BEGIN ROOT NODE
            DTB_NOP,
            DTB_NOP,
            DTB_BEGIN_NODE, 0, // cpus
                DTB_BEGIN_NODE, 0, // cpu0
                    DTB_NOP,
                    DTB_NOP,
                DTB_END_NODE,
                DTB_BEGIN_NODE, 0, // cpu1
                    DTB_NOP,
                    DTB_NOP,
                DTB_END_NODE,
            DTB_END_NODE,
            DTB_BEGIN_NODE, 0, // memory
                DTB_NOP,
                DTB_NOP,
            DTB_END_NODE,
        DTB_END
    };

    uint32_t *cpus_node = &tokenlist[4];
    uint32_t *memory_node = &tokenlist[17];
    uint32_t *result = skip_until_same_depth(cpus_node);
    if (result == memory_node) {
        printf("utils: skip_until_same_depth /cpus -> /memory " ANSI_COLOR_GREEN "SUCCESS" ANSI_COLOR_RESET "\n");
    } else {
        printf("utils: skip_until_same_depth /cpus -> /memory " ANSI_COLOR_RED "FAILED" ANSI_COLOR_RESET "\n");
        exit(1);
    }
}
