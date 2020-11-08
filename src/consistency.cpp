#include "priam/consistency.hpp"

namespace priam
{
using namespace std::string_literals;

static const std::string name_unknown      = "unknown"s;
static const std::string name_any          = "any"s;
static const std::string name_one          = "one"s;
static const std::string name_two          = "two"s;
static const std::string name_three        = "three"s;
static const std::string name_quorum       = "quorum"s;
static const std::string name_all          = "all"s;
static const std::string name_local_quorum = "local_quorum"s;
static const std::string name_each_quorum  = "each_quorum"s;
static const std::string name_serial       = "serial"s;
static const std::string name_local_serial = "local_serial"s;
static const std::string name_local_one    = "local_one"s;

auto to_string(consistency c) -> const std::string&
{
    switch (c)
    {
        case consistency::unknown:
            return name_unknown;
        case consistency::any:
            return name_any;
        case consistency::one:
            return name_one;
        case consistency::two:
            return name_two;
        case consistency::three:
            return name_three;
        case consistency::quorum:
            return name_quorum;
        case consistency::all:
            return name_all;
        case consistency::local_quorum:
            return name_local_quorum;
        case consistency::each_quorum:
            return name_each_quorum;
        case consistency::serial:
            return name_serial;
        case consistency::local_serial:
            return name_local_serial;
        case consistency::local_one:
            return name_local_one;
    }

    return name_unknown;
}

} // namespace priam
