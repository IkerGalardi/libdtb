#include "test.h"

int main(int argc, char **argv)
{
    assert((argc > 1));

    dtb *devicetree = dtb_fromptr(map_device_tree(argv[1]));
    if (devicetree == NULL) {
        printf("qemu-virt: dtb_fromptr " ANSI_COLOR_RED "FAILED\n" ANSI_COLOR_RESET);
        exit(1);
    } else {
        printf("qemu-virt: dtb_fromptr " ANSI_COLOR_GREEN "SUCCESS\n" ANSI_COLOR_RESET);
    }
}
