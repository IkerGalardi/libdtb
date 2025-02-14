# API

## Getting a device tree handle

```C
dtb *dtb_fromptr(void *ptr)
```
Returns a device tree handle after doing some checks.
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

```C
dtb_node dtb_find(dtb *devicetree, const char *path)
```
Returns a pointer to the node.
- `devicetree`: handle to the device tree.
- `path`: path to the device, does not need the unit address.
- *returns*: a pointer to the device node, `NULL` if it does not exist.

```C
dtb_node dtb_find_next(dtb_node node, const char *name)
```
Returns a pointer to the node.
- `node`: node to start searching from.
- `name`: name of the device, does not need the unit address.
- *returns*: a pointer to the device node, `NULL` if it does not exist.

```C
char *dtb_node_name(dtb_node node)
```
Returns a nul terminated string of the node name.
- `node`: pointer to a node.
- *returns*: nul terminated string.

```C
dtb_node dtb_first_child(dtb_node node)
```
Returns the first child node
- `node`: node to get the first child from.
- *returns*: first child node or `NULL` if it does not have any.

```C
dtb_node dtb_next_sibling(dtb_node node)
```
Returns the next sibling node
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

```C
dtb_property dtb_first_property(dtb_node node)
```
Returns the first property of the given node.
- `node`: node to take the first property from.
- *returns*: first property or `NULL` if it does not have any property.

```C
dtb_property dtb_next_property(dtb_property prop)
```
Returns the first property of the given node.
- `prop`: property to start searching from.
- *returns*: next property or `NULL` if it does not have any property.

```C
char *dtb_property_name(dtb_property prop)
```
Returns a nul terminated string of the property name.
- `prop`: property to get the name from
- *returns*: nul terminated string containing the name of the property.

```C
dtb_u32 dtb_property_uint32(dtb_property prop)
```
Get the 32 bit unsigned integer value
- `prop`: property to get the value from
- *returns*: value of the property.

```C
dtb_u64 dtb_property_uint64(dtb_property prop)
```
Get the 64 bit unsigned integer value
- `prop`: property to get the value from
- *returns*: value of the property.

```C
char *dtb_property_string(dtb_property prop)
```
Get the string value
- `prop`: property to get the value from
- *returns*: value of the property.

```C
char *dtb_property_array(dtb_property prop)
```
Get the array value
- `prop`: property to get the value from
- *returns*: value of the property.

### Examples

Iterating on properties of a node:
```C
dtb_property first = dtb_first_property(pcie_node);
for (dtb_property prop = first; prop != NULL; prop = dtb_next_property(prop))
    char *propname = dtb_property_name(property);
    if (strcmp(propname, "compatible") == 0) {
        char *compatible = dtb_property_string(property);
        printf("Device compatible with '%s\n'", compatible);
    } else if (strcmp(propname, "regs") == 0) {
        regs = (void *)dtb_property_array(property)
        printf("Device registers at %p\n", regs);
    } else if (strcmp(propname, "#address-cells") == 0) {
        dtb_u32 addr_cells = dtb_property_uint32(property);
        prinrtf("Device address cells of size %" PRIu32 "\n", addr_cells)
    }
}
```


## Reservation map

`dtb_rsvmap_entry`structure:
  - `dtb_u64 address`: address of the memory reservation.
  - `dtb_u64 size`: size of the memory reservation.

```C
dtb_rsvmap_entry *dtb_first_rsvmap_entry(dtb *devicetree)
```
Returns the first reservation entry
- `devicetree`: handle to the device tree.
- *returns*: first reservation map entry or `NULL` if there is not any.

```C
dtb_rsvmap_entry *dtb_next_rsvmap_entry(dtb_rsvmap_entry *entry)
```
Returns the next reservation entry
- `entry`: pointer to a reservation map entry
- *returns*: next reservation map entry or `NULL` if there is not any.

## Iterators

```C
dtb_foreach_child(node, name)
```
Iterate over device tree
- `node`: node to iterate childs on.
- `name`: name of the node variable.

```C
dtb_foreach_property(node, name)
```
Iterate over device tree
- `node`: node to iterate properties on.
- `name`: name of the property variable.

```C
dtb_foreach_rsvmap_entry(dtb, entry)
```
Iterate over device tree
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
