#pragma once

#include "catch.hpp"

#include <priam/CQL.hpp>

TEST_CASE("Create single keyspace")
{
    using namespace std::chrono_literals;
    auto cluster_ptr = priam::Cluster::make();
    cluster_ptr->AddHost("cassandra");
    cluster_ptr->SetPort(9042);
    priam::Client client{std::move(cluster_ptr), 10s};
    
    auto create_keyspace_prepared_ptr = client.CreatePrepared(
        "create keyspace single_keyspace",
        "CREATE KEYSPACE IF NOT EXISTS single_keyspace WITH REPLICATION = { 'class': 'SimpleStrategy', 'replication_factor': 1 }"
    );

    auto stmt_ptr = create_keyspace_prepared_ptr->CreateStatement();
    auto result = client.ExecuteStatement(std::move(stmt_ptr), 10s);

    CHECK(result.StatusCode() == CassError::CASS_OK);
}