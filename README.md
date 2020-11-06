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
* Prepared Statements only.
* Safe C++17 client library API, modern memory move semantics.
* Type Safe and easy conversions using Result/Row/Column objects to iterate over query results.
* Leverages the [Datastax](https://github.com/datastax/cpp-driver) C driver internally.  This library is compiled and statically linked in the default build.

# Usage #

## Examples

See all of the examples under the examples/ directory.
Below are some simple examples to get you started on using libpriamcql.

### Simple Synchronous Prepared Statement

```C++
// Start by creating a new cluster with settings on how to connect to Cassandra.
auto cluster_ptr = priam::Cluster::make();
cluster_ptr
    ->AddHost("localhost")
    .SetPort(9042)
    .SetUsernamePassword("bob", "password");

// Next create a client session to issue queries to Cassandra.  This requires
// moving ownership of the Cluster object into the Client instance.
auto client_ptr = std::make_unique<priam::Client>(std::move(cluster_ptr));

// Now create a prepared statement, we'll make a fake query that selects 'col1'
// from 'table_name' where its 'primary_key' will be bound to the prepared statement.
auto prepared_ptr = client_ptr->CreatePrepared("prepared_name", "SELECT col1 FROM table_name WHERE primary_key = ?");

// Create a statement from the prepared statement and bind the appropriate parameters.
auto statement_ptr = prepared_ptr->CreateStatement();
// Bind the 'primary_key' parameter, note that this can also be done by parameter index.
statement_ptr->BindInt("primary_key", 5);

// Finally execute the statement, again moving ownership into the client.
auto result = client_ptr->ExecuteStatement(
    std::move(statement_ptr),
    std::chrono::seconds{5},                    // An optional timeout.
    CassConsistency::CASS_CONSISTENCY_LOCAL_ONE // An optional query consistency.
);

// Now that we have the result we can work with the data.
std::cout << "Status code: " << priam::to_string(result.GetStatusCode) << std::endl;
std::cout << "Row count: " << result.GetRowCount() << std::endl;
std::cout << "Columns count: " << result.GetColumnCount() << std::endl;

result.ForEachRow(
    [](const priam::Row& row) -> void {
        row.ForEachColumn(const priam::Value& value) -> void {
            std::cout << "DataType: " << priam::to_string(value.GetDataType()) << std::endl;

            switch(value.GetDataType())
            {
                case CASS_VALUE_DOUBLE:
                    auto double_value = value.GetDouble();
                    break;
                // handle all values here or if you know the column type based
                // the query just extract based on the name
                case ...:
                    ...
                    break;
            }
        }
    }
);

// When Result destructs the query memory and resources are reclaimed by priam cql.

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
