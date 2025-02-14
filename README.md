# libdtb

libdtb is a small and portable and dependency free (not even the standard library) device tree
parser designed to be used on operating system kernels. This library implements node finding by path,
node finding by phandle, iterating on child nodes or iterating on node properties.

## Building

First you need to install any C compiler. Has been tested with GCC and Clang but any compile should
work.

Once the compiler its installed simply execute `make` in the terminal. The next are the built
artifacts:

- `libdtb-debug.a`: build with debug symbols.
- `libdtb.a`: release distribution.

## Integration

For an integration guide just read the [integration guide](docs/Integrating.md). For short, it is
recommended to simply integrate the files into the project.

## Contributing

The simplest way to contribute to the project is by proposing ideas on the issues tab or by
reporting bugs.

For the brave enough to write in C, I recommend first writing a feature request and then writing the
code in your own fork (Github pull request workflow). The next are some requirements:

- Commits should only represent a small change.
- Commit messages should summarize the change.
- Commit messages should be preceded by either *parser*, *test*, *meta*, *docs* or *tests*. See the
commit history for references.
