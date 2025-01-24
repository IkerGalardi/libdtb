#include "test.h"
#include "utils.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

   int result = strcmp_nodename("memory", "memory@800000");
   print_test_result("utils: strcmp_nodename 'memory' == 'memory@800000'",  result == 0);

   result = strcmp_nodename("soc", "soc");
   print_test_result("utils: strcmp_nodename 'soc' == 'soc'", result == 0);

   result = strcmp_nodename("memory", "soc");
   print_test_result("utils: strcmp_nodename 'memory' != 'soc'", result != 0);
}
