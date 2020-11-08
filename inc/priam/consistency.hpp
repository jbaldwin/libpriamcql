#pragma once

#include "priam/cpp_driver.hpp"

#include <string>

namespace priam
{
enum class consistency
{
    unknown      = CASS_CONSISTENCY_UNKNOWN,      //      = 0xFFFF,
    any          = CASS_CONSISTENCY_ANY,          //          = 0x0000,
    one          = CASS_CONSISTENCY_ONE,          //          = 0x0001,
    two          = CASS_CONSISTENCY_TWO,          //          = 0x0002,
    three        = CASS_CONSISTENCY_THREE,        //        = 0x0003,
    quorum       = CASS_CONSISTENCY_QUORUM,       //       = 0x0004,
    all          = CASS_CONSISTENCY_ALL,          //          = 0x0005,
    local_quorum = CASS_CONSISTENCY_LOCAL_QUORUM, // = 0x0006,
    each_quorum  = CASS_CONSISTENCY_EACH_QUORUM,  //  = 0x0007,
    serial       = CASS_CONSISTENCY_SERIAL,       //       = 0x0008,
    local_serial = CASS_CONSISTENCY_LOCAL_SERIAL, // = 0x0009,
    local_one    = CASS_CONSISTENCY_LOCAL_ONE,    //    = 0x000A
};

auto to_string(consistency c) -> const std::string&;

} // namespace priam
