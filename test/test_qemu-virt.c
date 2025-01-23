#include "test.h"
#include <string.h>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    dtb *devicetree = dtb_fromptr(map_device_tree("dtbfiles/qemu-virt.dtb"));
    print_test_result("qemu-virt: dtb_fromptr", devicetree != NULL);

    bool has_rsvmap_entries = false;
    dtb_foreach_rsvmap_entry(devicetree, {
        has_rsvmap_entries = true;
    });
    print_test_result("qemu-virt: dtb_foreach_rsvmap_entry", has_rsvmap_entries == false);

    dtb_node root_node = dtb_find(devicetree, "/");
    print_test_result("qemu-virt: dtb_find '/'", root_node != NULL);

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
    bool ok = found_addr_cell == true && found_size_cell == true && found_compatible == true
            && found_model == true;
    print_test_result("qemu-virt: dtb_foreach_property '/'", ok);

    dtb_node memory_node = dtb_find(devicetree, "/memory");
    print_test_result("qemu-virt: dtb_find '/memory'", memory_node != NULL);

    dtb_node serial_node = dtb_find(devicetree, "/soc/serial");
    print_test_result("qemu-virt: dtb_find '/soc/serial'", serial_node != NULL);
}
