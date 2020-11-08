#include "catch.hpp"

#include <priam/priam.hpp>

TEST_CASE("keyspace Create single keyspace")
{
    using namespace std::chrono_literals;
    auto cluster_ptr = priam::cluster::make_unique();
    cluster_ptr->add_host("cassandra");
    cluster_ptr->port(9042);
    priam::client client{std::move(cluster_ptr), 10s};

    {
        auto drop_keyspace_stmt = priam::statement{"DROP KEYSPACE IF EXISTS single_keyspace"};
        auto result             = client.execute_statement(drop_keyspace_stmt, 10s);
        REQUIRE(result.status() == priam::status::ok);
    }

    {
        priam::statement create_keyspace_stmt{
            "CREATE KEYSPACE IF NOT EXISTS single_keyspace WITH REPLICATION = { 'class': 'SimpleStrategy', 'replication_factor': 1 }"};

        auto result = client.execute_statement(create_keyspace_stmt, 10s);
        REQUIRE(result.status() == priam::status::ok);
    }
}
