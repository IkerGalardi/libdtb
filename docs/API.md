# API

## Getting a device tree handle

- `dtb *dtb_fromptr(void *ptr)`: returns a device tree handle after doing some checks.
  - `ptr`: pointer to the location the device tree resides.

### Examples

Getting a device tree handle from a pointer:
```C
    dtb *devicetree = dtb_fromptr(ptr);
    if (devicetree == NULL) {
        error...
    }
```

## Device nodes

- `dtb_node dtb_find(dtb *devicetree, const char *path)`: returns a pointer to the node.
  - `devicetree`: handle to the device tree.
  - `path`: path to the device, does not need the unit address.
  - *returns*: a pointer to the device node, `NULL` if it does not exist.

- `char *dtb_node_name(dtb_node node)`: returns a nul terminated string of the node name.
  - `node`: pointer to a node.
  - *returns*: nul terminated string.

- `dtb_node dtb_first_child(dtb_node node)`: returns the first child node
  - `node`: node to get the first child from.
  - *returns*: first child node or `NULL` if it does not have any.

- `dtb_node dtb_next_sibling(dtb_node node)`: returns the next sibling node
  - `node`: node to get the next sibling from.
  - *returns*: next sibling node or `NULL` if it does not have any.

### Examples

Finding the memory node:
```C
    dtb_node memory_node = dtb_find(devicetree, "/memory");
    if (memory_node == NULL) {
        error...
    }
```

Iterate on child nodes:
```C
    dtb_node first = dtb_first_child(soc_node);
    for (dtb_node node = first; node != NULL; node = dtb_next_sibling(node)) {
        char *devname = dtb_node_name(node);
        printf("Device '%s' available\n", devname);
    }
```

## Properties

- `dtb_property dtb_first_property(dtb_node node)`: returns the first property of the given node.
  - `node`: node to take the first property from.
  - *returns*: first property or `NULL` if it does not have any property.

- `dtb_property dtb_next_property(dtb_property prop)`: returns the first property of the given node.
  - `prop`: property to start searching from.
  - *returns*: next property or `NULL` if it does not have any property.

- `char *dtb_property_name(dtb_property prop)`: returns a nul terminated string of the property name.
  - `prop`: property to get the name from
  - *returns*: nul terminated string containing the name of the property.

- ` uint32_t dtb_property_uint32(dtb_property prop)`: get the 32 bit unsigned integer value
  - `prop`: property to get the value from
  - *returns*: value of the property.

- ` uint64_t dtb_property_uint64(dtb_property prop)`: get the 64 bit unsigned integer value
  - `prop`: property to get the value from
  - *returns*: value of the property.

- ` char *dtb_property_string(dtb_property prop)`: get the string value
  - `prop`: property to get the value from
  - *returns*: value of the property.

- ` char *dtb_property_array(dtb_property prop)`: get the array value
  - `prop`: property to get the value from
  - *returns*: value of the property.

## Reservation map

- `dtb_rsvmap_entry`structure:
  - `uint64_t address`: address of the memory reservation.
  - `uint64_t size`: size of the memory reservation.

- `dtb_rsvmap_entry *dtb_first_rsvmap_entry(dtb *devicetree)`: returns the first reservation entry
  - `devicetree`: handle to the device tree.
  - *returns*: first reservation map entry or `NULL` if there is not any.

- `dtb_rsvmap_entry *dtb_next_rsvmap_entry(dtb_rsvmap_entry *entry)`: returns the next reservation
entry
  - `entry`: pointer to a reservation map entry
  - *returns*: next reservation map entry or `NULL` if there is not any.

## Iterators

- `dtb_foreach_child(node, name)`: iterate over device tree
  - `node`: node to iterate childs on.
  - `name`: name of the node variable.

- `dtb_foreach_property(node, name)`: iterate over device tree
  - `node`: node to iterate properties on.
  - `name`: name of the property variable.

- `dtb_foreach_rsvmap_entry(dtb, entry)`: iterate over device tree
  - `dtb`: devicetree to iterate reservation map on.
  - `entry`: name of the entry variable.

### Examples

Iterating on the reservation map:
```C
    dtb_foreach_rsvmap_entry(devicetree, entry) {
        allocator_mark_reserved(entry.address, entry.size);
    }
```

Iterating on child nodes:
```C
    dtb_foreach_child(soc_node, dev) {
        printf("Found device %s\n", dtb_node_name(dev));
    }
```

Iterating on properties:
```C
    driver *drv = NULL;
    void *regs = NULL;
    dtb_foreach_property(device_node, property) {
        char *propname = dtb_property_name(property);

        if (strcmp(propname, "compatible") == 0) {
            char *compatible = dtb_property_string(property);
            drv = find_driver(compatible);
        } else if (strcmp(propname, "regs")) {
            regs = (void *)dtb_property_array(property)
        }
    }
    device *dev = create_device(drv, regs);
```
