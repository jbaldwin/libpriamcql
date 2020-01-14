#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <priam/CQL.hpp>

TEST_CASE("catch_is_working")
{
    auto cluster_ptr = priam::Cluster::make();
    CHECK(cluster_ptr != nullptr);
}
