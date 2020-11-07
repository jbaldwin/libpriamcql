#include "catch.hpp"

#include <priam/priam.hpp>

#include <iostream>

TEST_CASE("uuid_generator uuid_v1")
{
    priam::uuid_generator gen{};

    priam::uuid uuid_v1  = gen.uuid_v1();
    auto        uuid_str = priam::to_string(uuid_v1);

    std::cerr << "uuid_v1_str = " << uuid_str << "\n";
    REQUIRE(uuid_str.length() == 36);
    REQUIRE(uuid_str[uuid_str.length()] == '\0');
}

TEST_CASE("uuid_generator uuid_v1 with node")
{
    priam::uuid_generator gen{0xAAAABBBBCCCC};

    priam::uuid uuid_v1  = gen.uuid_v1();
    auto        uuid_str = priam::to_string(uuid_v1);

    std::cerr << "uuid_v1_str = " << uuid_str << "\n";
    REQUIRE(uuid_str.length() == 36);
    REQUIRE(uuid_str[uuid_str.length()] == '\0');
    REQUIRE(uuid_str.substr(36 - 12) == "aaaabbbbcccc");
}

TEST_CASE("uuid_generator uuid_v1 with epoch time stamp")
{
    priam::uuid_generator gen{1234};

    // Jan 1st 2000
    priam::uuid uuid_v1  = gen.uuid_v1(946684800ul);
    auto        uuid_str = priam::to_string(uuid_v1);

    std::cerr << "uuid_v1_str = " << uuid_str << "\n";
    REQUIRE(uuid_str.length() == 36);
    REQUIRE(uuid_str[uuid_str.length()] == '\0');
    /// 1234 is 0x4d2 in hex.
    REQUIRE(uuid_str.substr(33) == "4d2");
    REQUIRE(uuid_str.substr(0, 18) == "63b00000-bfde-11d3");
}

TEST_CASE("uuid_generator uuid_v4")
{
    priam::uuid_generator gen{};

    priam::uuid uuid_v4  = gen.uuid_v4();
    auto        uuid_str = priam::to_string(uuid_v4);
    std::cerr << "uuid_v4_str = " << uuid_str << "\n";
    REQUIRE(uuid_str.length() == 36);
    REQUIRE(uuid_str[uuid_str.length()] == '\0');
}
