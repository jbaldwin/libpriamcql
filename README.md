# libpriamcql - Safe Easy to use C++17 CQL Client library
=========================================================

[![CircleCI](https://circleci.com/gh/jbaldwin/libpriamcql/tree/master.svg?style=svg)](https://circleci.com/gh/jbaldwin/libpriamcql/tree/master)
[![language][badge.language]][language]
[![license][badge.license]][license]

[badge.language]: https://img.shields.io/badge/language-C%2B%2B17-yellow.svg
[badge.license]: https://img.shields.io/badge/license-Apache--2.0-blue

[language]: https://en.wikipedia.org/wiki/C%2B%2B17
[license]: https://en.wikipedia.org/wiki/Apache_License

https://github.com/jbaldwin/libpriamcql

**libpriamcql** is a C++17 client library that provides an easy, memory safe, and fast wrapper around the DataStax cpp-driver.

**libpriamcql** is licensed under the Apache 2.0 license.

# Overview #
* Easy to use Synchronous and Asynchronous CQL Query Request Support.
* Prepared Statements only.
* Safe C++17 client library API, modern memory move semantics.
* Type Safe and easy conversions using Result/Row/Column objects to iterate over query results.
* Leverages the [Datastax](https://github.com/datastax/cpp-driver) C driver internally.  This library is compiled and statically linked in the default build.

# Usage #

## Examples

See all of the examples under the examples/ directory.

simple examples TODO

## Requirements
    C++17 compiler (g++/clang++)
    CMake
    make and/or ninja
    pthreads/std::thread
    libuv devel
    openssl devel
    zlib devel

## Instructions

### Building
    # This will produce the libpriamcql.a and all of the examples
    # Since there are stl objects used in the API you will need to guarantee you are using
    # the same std library implementation as your application!

    mkdir Release && cd release
    cmake -DCMAKE_BUILD_TYPE=Release ..
    cmake --build . -- -j4 # change 4 to the number of cores available

## Support

File bug reports, feature requests and questions using [GitHub Issues](https://github.com/jbaldwin/libpriamcql/issues)

Copyright © 2017-2020, Josh Baldwin
