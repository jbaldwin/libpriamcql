# libpriamcql - Fast asynchronous C++17 CQL Client library
==========================================================

Copyright (c) 2018, Josh Baldwin

https://github.com/jbaldwin/libpriamcql

**libpriamcql** is a C++17 client library that provides an easy, memory safe, and fast wrapper around the DataStax cpp-driver.

**libpriamcql** is licensed under the Apache 2.0 license.

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
    # This will produce the libpriamcql.a and all of the examples
    # Since there are stl objects used in the API you will need to guarantee you are using
    # the same std library implementation as your application!

    mkdir Release && cd release
    cmake -DCMAKE_BUILD_TYPE=Release ..
    cmake --build . -- -j4 # change 4 to the number of cores available

## Examples

See all of the examples under the examples/ directory.

## Support

File bug reports, feature requests and questions using [GitHub Issues](https://github.com/jbaldwin/libpriamcql/issues)

