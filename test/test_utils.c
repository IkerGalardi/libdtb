#include "test.h"
#include "utils.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    {
        int result = strcmp_nodename("memory", "memory@800000");
        print_test_result("utils: strcmp_nodename 'memory' == 'memory@800000'",  result == 0);

        result = strcmp_nodename("soc", "soc");
        print_test_result("utils: strcmp_nodename 'soc' == 'soc'", result == 0);

        result = strcmp_nodename("memory", "soc");
        print_test_result("utils: strcmp_nodename 'memory' != 'soc'", result != 0);
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
    print_test_result("utils: skip_until_same_depth /cpus -> /memory", result == memory_node);

    uint32_t *cpu0_node = &tokenlist[6];
    uint32_t *cpu1_node = &tokenlist[11];
    result = skip_until_same_depth(cpu0_node);
    print_test_result("utils: skip_until_same_depth /cpus/cpu0 -> /cpus/cpu1", result == cpu1_node);

    result = skip_until_same_depth(cpu1_node);
    print_test_result("utils: skip_until_same_depth /cpus/cpu1 -> NULL", result == NULL);
}
