#include "test.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    dtb *devicetree = dtb_fromptr(map_device_tree("dtbfiles/qemu-a64virt.dtb"));

    dtb_node virtio_a00200 = dtb_find(devicetree, "/virtio_mmio@a000200");
    print_test_result("qemu-a64virt: dtb_find '/virtio_mmio@a000200'", virtio_a00200 != DTB_NULL);

    bool reg_ok = false;
    dtb_foreach_property(virtio_a00200, prop) {
        char *propname = dtb_property_name(devicetree, prop);

        if (strcmp(propname, "reg") == 0) {
            uint64_t regval = dtb_property_uint64(prop);
            reg_ok = regval == 0xa000200;
        }
    }

    print_test_result("qemu-a64virt: unaligned property 'reg'", reg_ok);
}
