#include "catch.hpp"

#include <priam/priam.hpp>

TEST_CASE("keyspace Create single keyspace")
{
    using namespace std::chrono_literals;
    auto cluster_ptr = priam::cluster::make_unique();
    cluster_ptr->add_host("cassandra");
    cluster_ptr->port(9042);
    priam::client client{std::move(cluster_ptr), 10s};

    auto create_keyspace_prepared_ptr = client.prepared_register(
        "create keyspace single_keyspace",
        "CREATE KEYSPACE IF NOT EXISTS single_keyspace WITH REPLICATION = { 'class': 'SimpleStrategy', 'replication_factor': 1 }");

    auto stmt_ptr = create_keyspace_prepared_ptr->create_statement();
    auto result   = client.execute_statement(std::move(stmt_ptr), 10s);

    CHECK(result.StatusCode() == CassError::CASS_OK);
}
