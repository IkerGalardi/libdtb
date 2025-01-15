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

    bool has_rsvmap_entries = false;
    dtb_foreach_rsvmap_entry(devicetree, {
        has_rsvmap_entries = true;
    });
    if (has_rsvmap_entries == true) {
        printf("qemu-virt: dtb_foreach_rsvmap_entry " ANSI_COLOR_RED "FAILED\n" ANSI_COLOR_RESET);
        exit(1);
    } else {
        printf("qemu-virt: dtb_foreach_rsvmap_entry " ANSI_COLOR_GREEN "SUCCESS\n" ANSI_COLOR_RESET);
    }

    dtb_node root_node = dtb_find(devicetree, "/");
    if (root_node == NULL) {
        printf("qemu-virt: dtb_find '/' " ANSI_COLOR_RED "FAILED\n" ANSI_COLOR_RESET);
        exit(1);
    } else {
        printf("qemu-virt: dtb_find '/' " ANSI_COLOR_GREEN "SUCCESS\n" ANSI_COLOR_RESET);
    }

    bool found_addr_cell = false;
    bool found_size_cell = false;
    bool found_compatible = false;
    bool found_model = false;
    dtb_foreach_property(devicetree, root_node, {
        if (strcmp(propname, "#address-cells") == 0) {
            found_addr_cell = true;
        } else if (strcmp(propname, "#size-cells") == 0) {
            found_size_cell = true;
        } else if (strcmp(propname, "compatible") == 0) {
            found_compatible = true;
        } else if (strcmp(propname, "model") == 0) {
            found_model = true;
        }
    });
    if (found_addr_cell == false || found_size_cell == false || found_compatible == false || found_model == false) {
        printf("qemu-virt: dtb_foreach_property '/' " ANSI_COLOR_RED "FAILED\n" ANSI_COLOR_RESET);
        exit(1);
    } else {
        printf("qemu-virt: dtb_foreach_property '/' " ANSI_COLOR_GREEN "SUCCESS\n" ANSI_COLOR_RESET);
    }

    dtb_node memory_node = dtb_find(devicetree, "/memory");
    if (memory_node == NULL) {
        printf("qemu-virt: dtb_find '/memory' " ANSI_COLOR_RED "FAILED\n" ANSI_COLOR_RESET);
        exit(1);
    } else {
        printf("qemu-virt: dtb_find '/memory' " ANSI_COLOR_RED "FAILED\n" ANSI_COLOR_RESET);
    }

    dtb_node serial_node = dtb_find(devicetree, "/soc/serial");
    if (serial_node == NULL) {
        printf("qemu-virt: dtb_find '/soc/serial' " ANSI_COLOR_RED "FAILED\n" ANSI_COLOR_RESET);
        exit(1);
    } else {
        printf("qemu-virt: dtb_find '/soc/serial' " ANSI_COLOR_GREEN "SUCCESS\n" ANSI_COLOR_RESET);
    }

}
