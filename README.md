libpriamcql - Safe Easy to use C++17 CQL Client library
=======================================================

[![CI](https://github.com/jbaldwin/libpriamcql/workflows/build/badge.svg)](https://github.com/jbaldwin/libpriamcql/workflows/build/badge.svg)
[![Coverage Status](https://coveralls.io/repos/github/jbaldwin/libpriamcql/badge.svg?branch=master)](https://coveralls.io/github/jbaldwin/libpriamcql?branch=master)
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
* Supports ad-hoc queries and prepared statements.
* Safe C++17 client library API, modern memory move semantics.
* Type Safe and easy conversions using Result/Row/Column objects to iterate over query results.
* Leverages the [Datastax](https://github.com/datastax/cpp-driver) C driver internally.  This library is compiled and statically linked in the default build.

# Usage #

## Examples

See all of the examples under the examples/ directory.
Below are some simple examples to get you started on using libpriamcql.

### Simple Synchronous Prepared Statement

```C++
#include <priam/priam.hpp>

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

int main()
{
    // Start by creating a new cluster with settings on how to connect to Cassandra.
    auto cluster_ptr = priam::cluster::make_unique();
    cluster_ptr
        ->add_host("localhost")
        .port(9042)
        .username_and_password("username", "password");

    // Next create a client session to issue queries to Cassandra.  This requires
    // moving ownership of the Cluster object into the Client instance.
    auto client_ptr = std::make_unique<priam::client>(std::move(cluster_ptr));

    // Create a statement with a single primary key to be bound.
    priam::statement stmt{"SELECT val1, val2 FROM table_name WHERE primary_key = ?"};
    // Bind the 'primary_key' parameter, note that this can also be done by parameter index.
    stmt.bind_int(5, "primary_key");

    // Execute the statement synchronously, async queries are also supported.
    auto result = client_ptr->execute_statement(
        stmt,                           // The statement to execute, can be re-used via reset().
        std::chrono::seconds{5},        // An optional timeout.
        priam::consistency::local_one   // An optional query consistency.
    );

    // Now that we have the result we can work with the data.
    std::cout << "Status code: " << priam::to_string(result.status()) << "\n";
    std::cout << "Row count: " << result.row_count() << "\n";
    std::cout << "Columns count: " << result.column_count() << "\n";

    for(const auto& row : result)
    {
        auto val1 = row[0];         // Fetch column value by name.
        auto val2 = row["val2"];    // Fetch column value by index.

        // All the returned columns can also be iterator over.
        for(const auto& value : row)
        {
            switch(value.type())
            {
                case priam::data_type::int_t:
                    // All values in C* can be nullable and are returned as an optional.
                    std::cout << "int value = " << value.as_int().value_or(0) << "\n";
                    break;
            }

            if(value.is<priam::data_type::boolean>())
            {
                std::cout << "bool value = " << value.as_boolean().value_or(false) << "\n";
            }
        }
    }

    return 0;
}
```

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
    # This will produce the libpriamcql.a and all of the examples and tests.

    mkdir Release && cd Release
    cmake -DCMAKE_BUILD_TYPE=Release ..
    cmake --build . -- -j$(nproc)

## Support

File bug reports, feature requests and questions using [GitHub Issues](https://github.com/jbaldwin/libpriamcql/issues)

Copyright © 2017-2020, Josh Baldwin
