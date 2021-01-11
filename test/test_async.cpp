#include "catch.hpp"

#include <priam/priam.hpp>

#include <iostream>

using namespace std::chrono_literals;

static auto create_keyspace(priam::client& client) -> void
{
    priam::statement stmt{
        "CREATE KEYSPACE IF NOT EXISTS test_async WITH REPLICATION = { 'class': 'SimpleStrategy', 'replication_factor': 1 }"};

    auto result = client.execute_statement(stmt, 10s);
    REQUIRE(result.status() == priam::status::ok);
}

static auto drop_keyspace(priam::client& client) -> void
{
    priam::statement stmt{"DROP KEYSPACE IF EXISTS test_async"};
    auto             result = client.execute_statement(stmt, 10s);
    REQUIRE(result.status() == priam::status::ok);
}

static auto create_table(priam::client& client, std::string_view query) -> void
{
    priam::statement stmt{query};
    auto             result = client.execute_statement(stmt, 10s);
    REQUIRE(result.status() == priam::status::ok);
}

TEST_CASE("async resuse statement")
{
    auto cluster_ptr = priam::cluster::make_unique();
    cluster_ptr->add_host("cassandra").port(9042);
    priam::client client{std::move(cluster_ptr), 10s};

    drop_keyspace(client);
    create_keyspace(client);
    create_table(
        client, "CREATE TABLE IF NOT EXISTS test_async.requests (key int, value text, PRIMARY KEY (key))");

    priam::statement insert{"INSERT INTO test_async.requests (key, value) VALUES (?, ?)"};

    auto insert_result_func = [&](int i, priam::result r) -> void {
        REQUIRE(r.status() == priam::status::ok);

        for(const auto& row : result)
        {
            
        }
    };

    auto insert_func = [&](int key, std::string_view value) -> void {
        REQUIRE(insert.bind_int(key, 0) == priam::status::ok);
        REQUIRE(insert.bind_text(value, value) == priam::status::ok);
        client.execute_statement(insert, 10s, [&](priam::result r) { insert_result_func(key, std::move(r)));
        insert.reset();
    };

    for(size_t i = 0; i < 1000; ++i)
    {
        insert_func(i, std::to_string(i));
    }

    priam::statement select{"SELECT value FROM test_async.requests WHERE key = ?"};

    auto select_result_func

    auto select_func = [&](int key) -> void {
        REQUIRE(select.bind_int(key) == priam::status::ok);
        auto result = client.execute_statement(select);
        REQUIRE(result.status() == priam::status::ok);
        select.reset();
    };
}
