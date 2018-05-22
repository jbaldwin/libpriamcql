# libvidarcql - Fast asynchronous C++17 CQL Client library
==========================================================

The god Odin was devoured by the wolf Fenrir. Vidar, a son of Odin by the giantess Gríðr, immediately set upon the wolf to avenge his father's death. He wore a shoe that had been crafted for this particular moment. It was the strongest and sturdiest of all shoes, and surely also charged with magical properties.

Alright, this isn't really Greek/Cassandra related but more about what this client library does!

Copyright (c) 2018, Josh Baldwin

https://github.com/jbaldwin/libvidarcql

**libvidarcql** is a C++17 client library that provides an easy, memory safe, and fast wrapper around the DataStax cpp-driver.

**libvidarcql** is licensed under the Apache 2.0 license.

# Overview #
* Asynchronous CQL Query Request Support.
* Prepared Statements only.
* C++17 Memory safe ownership around the underlying cpp-driver C code.
* Type Safe and easy conversions using Result/Row/Column objects to iterate over results.

# Usage #

## Requirements
  https://github.com/datastax/cpp-driver You'll need the devel packages to compile against.
  C++17 compiler (g++/clang++)
  CMake

## Instructions

### Building
    # This will produce the libvidarcql.a and all of the examples
    mkdir Release && cd release
    cmake -DCMAKE_BUILD_TYPE=Release ..
    cmake --build . -- -j4 # change 4 to the number of cores available

## Examples

See all of the examples under the examples/ directory.

## Support

File bug reports, feature requests and questions using [GitHub Issues](https://github.com/jbaldwin/libvidarcql/issues)

