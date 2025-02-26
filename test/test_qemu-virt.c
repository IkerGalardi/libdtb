#include "test.h"
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

dtb *devicetree;

static dtb_node test_dtb_find(const char *name, const char *complete_name)
{
    dtb_node node = dtb_find(devicetree, name);
    bool ok = node != NULL && strcmp(dtb_node_name(node), complete_name) == 0;

    char testname[100] = {0};
    snprintf(testname, 100, "qemu-virt: dtb_find '%s'", name);
    print_test_result(testname, ok);

    return node;
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    devicetree = dtb_fromptr(map_device_tree("dtbfiles/qemu-virt.dtb"));
    print_test_result("qemu-virt: dtb_fromptr", devicetree != NULL);

    bool has_rsvmap_entries = false;
    dtb_foreach_rsvmap_entry(devicetree, entry) {
        has_rsvmap_entries = true;
    }
    print_test_result("qemu-virt: dtb_foreach_rsvmap_entry", has_rsvmap_entries == false);

    dtb_node root_node = dtb_find(devicetree, "/");
    print_test_result("qemu-virt: dtb_find '/'", root_node != NULL);

    test_dtb_find("/pmu", "pmu");
    test_dtb_find("/fw-cfg", "fw-cfg@10100000");
    test_dtb_find("/flash", "flash@20000000");
    test_dtb_find("/chosen", "chosen");
    test_dtb_find("/poweroff", "poweroff");
    test_dtb_find("/reboot", "reboot");
    test_dtb_find("/platform-bus", "platform-bus@4000000");
    test_dtb_find("/memory", "memory@80000000");
    test_dtb_find("/cpus", "cpus");
    test_dtb_find("/cpus/cpu@0", "cpu@0");
    test_dtb_find("/cpus/cpu-map", "cpu-map");
    test_dtb_find("/cpus/cpu-map/cluster0", "cluster0");
    test_dtb_find("/cpus/cpu-map/cluster0/core0", "core0");
    test_dtb_find("/soc", "soc");
    test_dtb_find("/soc/rtc", "rtc@101000");
    test_dtb_find("/soc/serial", "serial@10000000");
    test_dtb_find("/soc/test", "test@100000");
    test_dtb_find("/soc/pci", "pci@30000000");
    test_dtb_find("/soc/virtio_mmio@10008000", "virtio_mmio@10008000");
    test_dtb_find("/soc/virtio_mmio@10007000", "virtio_mmio@10007000");
    test_dtb_find("/soc/virtio_mmio@10006000", "virtio_mmio@10006000");
    test_dtb_find("/soc/virtio_mmio@10005000", "virtio_mmio@10005000");
    test_dtb_find("/soc/virtio_mmio@10004000", "virtio_mmio@10004000");
    test_dtb_find("/soc/virtio_mmio@10003000", "virtio_mmio@10003000");
    test_dtb_find("/soc/virtio_mmio@10002000", "virtio_mmio@10002000");
    test_dtb_find("/soc/virtio_mmio@10001000", "virtio_mmio@10001000");
    test_dtb_find("/soc/plic", "plic@c000000");
    test_dtb_find("/soc/clint", "clint@2000000");

    bool correct_addr_cells = false;
    bool correct_size_cells = false;
    bool correct_compatible = false;
    bool correct_model = false;
    dtb_foreach_property(root_node, prop) {
        char *propname = dtb_property_name(devicetree, prop);

        if (strcmp(propname, "#address-cells") == 0) {
            correct_addr_cells = dtb_property_uint32(prop) == 0x2;
        } else if (strcmp(propname, "#size-cells") == 0) {
            correct_size_cells = dtb_property_uint32(prop) == 0x2;
        } else if (strcmp(propname, "compatible") == 0) {
            correct_compatible = strcmp(dtb_property_string(prop), "riscv-virtio") == 0;
        } else if (strcmp(propname, "model") == 0) {
            correct_model = strcmp(dtb_property_string(prop), "riscv-virtio,qemu") == 0;
        }
    }
    print_test_result("qemu-virt: '/' property '#address-cells'", correct_addr_cells);
    print_test_result("qemu-virt: '/' property '#size-cells'", correct_size_cells);
    print_test_result("qemu-virt: '/' property 'compatible'", correct_compatible);
    print_test_result("qemu-virt: '/' property 'model'", correct_model);
}
