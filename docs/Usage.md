# Usage

This library exposes ways to find and iterate on both nodes and properties of a node. For now only
reading is supported.

To start using the library, first you need to get a device tree handle by calling the `dtb_fromptr`
function. This function will verify that the passed pointer corresponds to an actual device tree by
checking the magic value and that the passed device tree is a supported one.

```C
    dtb *devicetree = dtb_fromptr(pointer);
```

In case there is any error the function will return `NULL` in order to signal the error. So it is
important to check that.

After that, the next step is to find the appropriate node in order to parse its properties or child
nodes. For that we need to use the `dtb_find` node.

```C
    dtb_node memory_node = dtb_find(devicetree, "/memory");
```

The passed path does not need to include any unit address in order to find the node. In case there
are multiple nodes with the same name only the first one is returned. As with the `dtb_fromptr`
function, this one also signals errors by returning `NULL`.

To iterate the properties of a node, you can iterate it manually by using the `dtb_first_property`
and `dtb_next_property` functions, but its recommended to use the `dtb_foreach_property` macro to
simplify its use (and reduce line length).

```C
    dtb_foreach_property(devicetree, prop) {
        char *propname = dtb_property_name(devicetree, prop);
        printf("%s\n", propname);
    }
```
The next are the available getters for a property:

- `dtb_property_name`: used to retrieve the a string containing the property name (unit address
included.)
- `dtb_property_uint32`: get the value of the property as a 32 bit unsigned integer.
- `dtb_property_uint64`: get the value of the property as a 64 bit unsigned integer.
- `dtb_property_string`: get the value of the property as a string.
- `dtb_property_array`: get the value of the property as an array.

It is important to note that the library does not do any type checking and simply returns the value
(also does the big-endian to little-endian conversion).

The library also lets you iterate on reservation maps using `dtb_foreach_rsvmap_entry` and child
nodes by using `dtb_foreach_child`.
