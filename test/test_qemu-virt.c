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

    dtb_node pmu_node = test_dtb_find("/pmu", "pmu");
    dtb_node fw_cfg = test_dtb_find("/fw-cfg", "fw-cfg@10100000");
    dtb_node flash = test_dtb_find("/flash", "flash@20000000");
    dtb_node chosen = test_dtb_find("/chosen", "chosen");
    dtb_node poweroff = test_dtb_find("/poweroff", "poweroff");
    dtb_node reboot = test_dtb_find("/reboot", "reboot");
    dtb_node platform_bus = test_dtb_find("/platform-bus", "platform-bus@4000000");
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

    bool found_riscv_evt_mhpmcounters = false;
    correct_compatible = false;
    dtb_foreach_property(pmu_node, prop) {
        char *propname = dtb_property_name(devicetree, prop);

        if (strcmp(propname, "riscv,event-to-mhpmcounters") == 0) {
            found_riscv_evt_mhpmcounters = true;
        } else if (strcmp(propname, "compatible") == 0) {
            correct_compatible = strcmp(dtb_property_string(prop), "riscv,pmu") == 0;
        }
    }
    print_test_result("qemu-virt: '/pmu' property 'riscv,event-to-mhpmcounters'", found_riscv_evt_mhpmcounters);
    print_test_result("qemu-virt: '/pmu' property 'compatible'", correct_compatible);

    bool found_dma_coherent = false;
    bool found_reg = false;
    correct_compatible = false;
    dtb_foreach_property(fw_cfg, prop) {
        char *propname = dtb_property_name(devicetree, prop);

        if (strcmp(propname, "dma-coherent") == 0) {
            found_dma_coherent = true;
        } else if (strcmp(propname, "reg") == 0) {
            found_reg = true;
        } else if (strcmp(propname, "compatible") == 0) {
            correct_compatible = strcmp(dtb_property_string(prop), "qemu,fw-cfg-mmio") == 0;
        }
    }
    print_test_result("qemu-virt: '/fw-cfg' property 'dma-coherent'", found_dma_coherent);
    print_test_result("qemu-virt: '/fw-cfg' property 'reg'", found_reg);
    print_test_result("qemu-virt: '/fw-cfg' property 'compatible'", correct_compatible);

    bool correct_bank_width = false;
    found_reg = false;
    correct_compatible = false;
    dtb_foreach_property(flash, prop) {
        char *propname = dtb_property_name(devicetree, prop);

        if (strcmp(propname, "bank-width") == 0) {
            correct_bank_width = dtb_property_uint32(prop) == 0x04;
        } else if (strcmp(propname, "reg") == 0) {
            found_reg = true;
        } else if (strcmp(propname, "compatible") == 0) {
            correct_compatible = strcmp(dtb_property_string(prop), "cfi-flash") == 0;
        }
    }
    print_test_result("qemu-virt: '/flash' property 'bank-width'", correct_bank_width);
    print_test_result("qemu-virt: '/flash' property 'reg'", found_reg);
    print_test_result("qemu-virt: '/flash' property 'compatible'", correct_compatible);

    bool found_rng_seed = false;
    bool correct_stdout_path = false;
    dtb_foreach_property(chosen, prop) {
        char *propname = dtb_property_name(devicetree, prop);

        if (strcmp(propname, "rng-seed") == 0) {
            found_rng_seed = true;
        } else if (strcmp(propname, "stdout-path") == 0) {
            correct_stdout_path = strcmp(dtb_property_string(prop), "/soc/serial@10000000") == 0;
        }
    }
    print_test_result("qemu-virt: '/chosen' property 'rng-seed'", found_rng_seed);
    print_test_result("qemu-virt: '/chosen' property 'stdout-path'", correct_stdout_path);

    bool correct_value = false;
    bool correct_offset = false;
    bool correct_regmap = false;
    correct_compatible = false;
    dtb_foreach_property(poweroff, prop) {
        char *propname = dtb_property_name(devicetree, prop);

        if (strcmp(propname, "value") == 0) {
            correct_value = dtb_property_uint32(prop) == 0x5555;
        } else if (strcmp(propname, "offset") == 0) {
            correct_offset = dtb_property_uint32(prop) == 0x0;
        } else if (strcmp(propname, "regmap") == 0) {
            correct_regmap = dtb_property_uint32(prop) == 0x04;
        } else if (strcmp(propname, "compatible") == 0) {
            correct_compatible = strcmp(dtb_property_string(prop), "syscon-poweroff") == 0;
        }
    }
    print_test_result("qemu-virt: '/poweroff' property 'value'", correct_value);
    print_test_result("qemu-virt: '/poweroff' property 'offset'", correct_offset);
    print_test_result("qemu-virt: '/poweroff' property 'regmap'", correct_regmap);
    print_test_result("qemu-virt: '/poweroff' property 'compatible'", correct_compatible);

    correct_value = false;
    correct_offset = false;
    correct_regmap = false;
    correct_compatible = false;
    dtb_foreach_property(reboot, prop) {
        char *propname = dtb_property_name(devicetree, prop);

        if (strcmp(propname, "value") == 0) {
            correct_value = dtb_property_uint32(prop) == 0x7777;
        } else if (strcmp(propname, "offset") == 0) {
            correct_offset = dtb_property_uint32(prop) == 0x0;
        } else if (strcmp(propname, "regmap") == 0) {
            correct_regmap = dtb_property_uint32(prop) == 0x04;
        } else if (strcmp(propname, "compatible") == 0) {
            correct_compatible = strcmp(dtb_property_string(prop), "syscon-reboot") == 0;
        }
    }
    print_test_result("qemu-virt: '/reboot' property 'value'", correct_value);
    print_test_result("qemu-virt: '/reboot' property 'offset'", correct_offset);
    print_test_result("qemu-virt: '/reboot' property 'regmap'", correct_regmap);
    print_test_result("qemu-virt: '/reboot' property 'compatible'", correct_compatible);

    bool correct_interrupt_parent = false;
    bool found_ranges = false;
    correct_addr_cells = false;
    correct_size_cells = false;
    correct_compatible = false;
    dtb_foreach_property(platform_bus, prop) {
        char *propname = dtb_property_name(devicetree, prop);

        if (strcmp(propname, "interrupt-parent") == 0) {
            correct_interrupt_parent = dtb_property_uint32(prop) == 0x03;
        } else if (strcmp(propname, "ranges") == 0) {
            found_ranges = true;
        } else if (strcmp(propname, "#address-cells") == 0) {
            correct_addr_cells = dtb_property_uint32(prop) == 0x01;
        } else if (strcmp(propname, "#size-cells") == 0) {
            correct_size_cells = dtb_property_uint32(prop) == 0x01;
        } else if (strcmp(propname, "compatible") == 0) {
            bool found_qemu = false;
            bool found_simple = false;
            dtb_foreach_stringlist(prop, name) {
                if (strcmp(name, "qemu,platform") == 0) {
                    found_qemu = true;
                } else if (strcmp(name, "simple-bus") == 0) {
                    found_simple = true;
                }
            }
            correct_compatible = found_qemu && found_simple;
        }
    }
    print_test_result("qemu-virt: '/platform-bus' property 'interrupt-parent'", correct_interrupt_parent);
    print_test_result("qemu-virt: '/platform-bus' property 'ranges'", found_ranges);
    print_test_result("qemu-virt: '/platform-bus' property '#address-cells'", correct_addr_cells);
    print_test_result("qemu-virt: '/platform-bus' property '#size-cells'", correct_size_cells);
    print_test_result("qemu-virt: '/platform-bus' property 'compatible'", correct_compatible);
}
