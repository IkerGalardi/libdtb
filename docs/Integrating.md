# Integrating

Integrating this library in any userspace is simple, as by executing `make` on the root of the
project yields a static library called `libdtb.a` (or `libdtb-debug.a` for the version with symbols).

For integrating on more custom environments like operating system kernels where the standard library
is not available the next steps should be followed:
1. Modify integer typedefs at the start of `dtb.h`.
2. Include the source files to the project and compile it directly in the project.

To reduce the number of included files the utility functions can be included directly in the `dtb.c`
file as static functions.
