#include "catch.hpp"

#include <priam/priam.hpp>

#include <iostream>

using namespace std::chrono_literals;

static auto create_keyspace(priam::client& client) -> void
{
    priam::statement stmt{
        "CREATE KEYSPACE IF NOT EXISTS test_types WITH REPLICATION = { 'class': 'SimpleStrategy', 'replication_factor': 1 }"};

    auto result = client.execute_statement(stmt, 10s);
    REQUIRE(result.status() == priam::status::ok);
}

static auto drop_keyspace(priam::client& client) -> void
{
    priam::statement stmt{"DROP KEYSPACE IF EXISTS test_types"};
    auto             result = client.execute_statement(stmt, 10s);
    REQUIRE(result.status() == priam::status::ok);
}

static auto create_table(priam::client& client, std::string_view query) -> void
{
    priam::statement stmt{query};
    auto             result = client.execute_statement(stmt, 10s);
    REQUIRE(result.status() == priam::status::ok);
}

TEST_CASE("type boolean")
{
    auto cluster_ptr = priam::cluster::make_unique();
    cluster_ptr->add_host("cassandra").port(9042);
    priam::client client{std::move(cluster_ptr), 10s};

    drop_keyspace(client);
    create_keyspace(client);
    create_table(
        client, "CREATE TABLE IF NOT EXISTS test_types.test_boolean (key int, value boolean, PRIMARY KEY (key))");

    priam::statement insert{"INSERT INTO test_types.test_boolean (key, value) VALUES (?, ?)"};
    auto             insert_func = [&](int key, bool value) -> void {
        REQUIRE(insert.bind_int(key, 0) == priam::status::ok);
        REQUIRE(insert.bind_boolean(value, 1) == priam::status::ok);
        auto result = client.execute_statement(insert);
        REQUIRE(result.status() == priam::status::ok);
        insert.reset();
    };

    insert_func(1, true);
    insert_func(2, false);
    insert_func(3, false);
    insert_func(4, true);
    insert_func(5, true);

    priam::statement select{"SELECT key, value FROM test_types.test_boolean WHERE key = ?"};
    auto             select_func = [&](int key, std::optional<bool> expected_value) -> void {
        select.bind_int(key, 0);
        auto result = client.execute_statement(select);
        REQUIRE(result.status() == priam::status::ok);

        result.for_each([&](const priam::row& row) {
            row.for_each([&](const priam::value& value) {
                if (value.is<priam::data_type::int_t>())
                {
                    REQUIRE(value.as_int().value() == key);
                }
                else
                {
                    REQUIRE(value.is<priam::data_type::boolean>());

                    if (expected_value.has_value())
                    {
                        REQUIRE(value.as_boolean().value() == expected_value.value());
                    }
                    else
                    {
                        REQUIRE(value.is_null());
                    }
                }
            });
        });
    };

    select_func(1, {true});
    select_func(2, {false});
    select_func(3, {false});
    select_func(4, {true});
    select_func(5, {true});
    select_func(6, std::nullopt);

    {
        priam::statement select_all{"SELECT key, value FROM test_types.test_boolean WHERE key IN (1, 2, 3, 4, 5, 6)"};
        auto             result = client.execute_statement(select_all);
        REQUIRE(result.status() == priam::status::ok);

        for (const auto& row : result)
        {
            auto key   = row["key"];
            auto value = row.column(1);

            REQUIRE(key.is<priam::data_type::int_t>());
            REQUIRE_FALSE(key.is_null());

            REQUIRE(value.is<priam::data_type::boolean>());
            if (value.is_null())
            {
                REQUIRE(key.as_int().value() == 6);
            }
            else
            {
                int key_value = key.as_int().value();
                REQUIRE(key_value >= 1);
                REQUIRE(key_value <= 5);

                auto value_value = value.as_boolean().value();
                if (key_value == 2 || key_value == 3)
                {
                    REQUIRE(value_value == false);
                }
                else
                {
                    REQUIRE(value_value == true);
                }
            }
        }

        for (const auto& row : result)
        {
            for (const auto& value : row)
            {
                auto k = row["key"];
                auto v = row["value"];

                if (value.is<priam::data_type::int_t>())
                {
                    REQUIRE_FALSE(value.is_null());
                }
                else if (value.is<priam::data_type::boolean>())
                {
                    if (value.is_null())
                    {
                        REQUIRE(k.as_int().value() == 6);
                    }
                    else
                    {
                        int k_value = k.as_int().value();
                        REQUIRE(k_value >= 1);
                        REQUIRE(k_value <= 5);

                        auto v_value = value.as_boolean().value();
                        if (k_value == 2 || k_value == 3)
                        {
                            REQUIRE(v_value == false);
                        }
                        else
                        {
                            REQUIRE(v_value == true);
                        }
                    }
                }
                else
                {
                    REQUIRE(false);
                }
            }
        }
    }
}