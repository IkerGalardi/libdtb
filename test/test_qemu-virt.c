#include "test.h"
#include <inttypes.h>
#include <string.h>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    dtb *devicetree = dtb_fromptr(map_device_tree("dtbfiles/qemu-virt.dtb"));
    print_test_result("qemu-virt: dtb_fromptr", devicetree != NULL);

    bool has_rsvmap_entries = false;
    dtb_foreach_rsvmap_entry(devicetree, entry) {
        has_rsvmap_entries = true;
    }
    print_test_result("qemu-virt: dtb_foreach_rsvmap_entry", has_rsvmap_entries == false);

    dtb_node null_node = dtb_find(devicetree, "/not/exist");
    print_test_result("qemu-virt: dtb_find '/not/exist'", null_node == NULL);

    dtb_node root_node = dtb_find(devicetree, "/");
    print_test_result("qemu-virt: dtb_find '/'", root_node != NULL);

    bool found_compatible = false;
    bool found_model = false;
    bool model_ok = false;
    bool found_addr_cells = false;
    bool addr_cells_ok = false;
    bool found_size_cells = false;
    bool size_cells_ok = false;
    dtb_foreach_property(root_node, prop) {
        char *propname = dtb_property_name(devicetree, prop);
        if (strcmp(propname, "compatible") == 0) {
            found_compatible = true;
        } else if (strcmp(propname, "model") == 0) {
            char *model_str = dtb_property_string(prop);
            model_ok = strcmp(model_str, "riscv-virtio,qemu") == 0;
            found_model = true;
        } else if (strcmp(propname, "#address-cells") == 0) {
            addr_cells_ok = dtb_property_uint32(prop) == 0x2;
            found_addr_cells = true;
        } else if (strcmp(propname, "#size-cells") == 0) {
            size_cells_ok = dtb_property_uint32(prop) == 0x2;
            found_size_cells = true;
        }
    }
    bool ok = found_compatible && found_model && found_addr_cells && found_size_cells;
    print_test_result("qemu-virt: dtb_foreach_property '/'", ok);
    print_test_result("qemu-virt: dtb_property_uint32 '/soc:#address-cells' == 2", addr_cells_ok && size_cells_ok);
    print_test_result("qemu-virt: dtb_property_str '/soc:model' == 'riscv-virtio,qemu'", model_ok);

    bool found_pmu = false;
    bool found_fw_cfg = false;
    bool found_flash = false;
    bool found_chosen = false;
    bool found_poweroff = false;
    bool found_reboot = false;
    bool found_platform_bus = false;
    bool found_memory = false;
    bool found_cpus = false;
    bool found_soc = false;
    dtb_foreach_child(root_node, child) {
        char *childname = dtb_node_name(child);
        if (strcmp(childname, "pmu") == 0) {
            found_pmu = true;
        } else if (strcmp(childname, "fw-cfg@10100000") == 0) {
            found_fw_cfg = true;
        } else if (strcmp(childname, "flash@20000000") == 0) {
            found_flash = true;
        } else if (strcmp(childname, "chosen") == 0) {
            found_chosen = true;
        } else if (strcmp(childname, "poweroff") == 0) {
            found_poweroff = true;
        } else if (strcmp(childname, "reboot") == 0) {
            found_reboot = true;
        } else if (strcmp(childname, "platform-bus@4000000") == 0) {
            found_platform_bus = true;
        } else if (strcmp(childname, "memory@80000000") == 0) {
            found_memory = true;
        } else if (strcmp(childname, "cpus") == 0) {
            found_cpus = true;
        } else if (strcmp(childname, "soc") == 0) {
            found_soc = true;
        }
    }
    ok = found_pmu && found_fw_cfg && found_flash && found_chosen && found_poweroff && found_reboot
        && found_platform_bus && found_memory && found_cpus && found_soc;
    print_test_result("qemu-virt: dtb_foreach_child '/'", ok);

    dtb_node memory_node = dtb_find(devicetree, "/memory");
    ok = memory_node != NULL && strcmp(dtb_node_name(memory_node), "memory@80000000") == 0;
    print_test_result("qemu-virt: dtb_find '/memory'", ok);

    bool found_device_type = false;
    bool found_reg = true;
    dtb_foreach_property(memory_node, prop) {
        char *propname = dtb_property_name(devicetree, prop);

        if (strcmp(propname, "device_type") == 0) {
            found_device_type = true;
        } else if (strcmp(propname, "reg") == 0) {
            found_reg = true;
        }
    }
    ok = found_device_type && found_reg;
    print_test_result("qemu-virt: dtb_foreach_property '/memory'", ok);

    dtb_node serial_node = dtb_find(devicetree, "/soc/serial");
    ok = memory_node != NULL && strcmp(dtb_node_name(serial_node), "serial@10000000") == 0;
    print_test_result("qemu-virt: dtb_find '/soc/serial'", ok);

    dtb_node cpus_node = dtb_next_sibling(memory_node);
    char *cpus_node_str = dtb_node_name(cpus_node);
    print_test_result("qemu-virt: dtb_next_sibling '/memory' -> '/cpus'", strcmp(cpus_node_str, "cpus") == 0);

    dtb_node test_node = dtb_next_sibling(serial_node);
    char *test_node_str = dtb_node_name(test_node);
    print_test_result("qemu-virt: dtb_next_sibling '/soc/serial' -> '/soc/test'", strcmp(test_node_str, "test@100000") == 0);
}
