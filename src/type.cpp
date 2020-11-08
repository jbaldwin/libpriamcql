#include "priam/type.hpp"

namespace priam
{
using namespace std::string_literals;

static const std::string custom_name    = "custom"s;
static const std::string ascii_name     = "ascii"s;
static const std::string bigint_name    = "bigint"s;
static const std::string blob_name      = "blob"s;
static const std::string boolean_name   = "boolean"s;
static const std::string counter_name   = "counter"s;
static const std::string decimal_name   = "decimal"s;
static const std::string double_name    = "double"s;
static const std::string float_name     = "float"s;
static const std::string int_name       = "int"s;
static const std::string text_name      = "text"s;
static const std::string timestamp_name = "timestamp"s;
static const std::string uuid_name      = "uuid"s;
static const std::string varchar_name   = "varchar"s;
static const std::string varint_name    = "varint"s;
static const std::string timeuuid_name  = "timeuuid"s;
static const std::string inet_name      = "inet"s;
static const std::string date_name      = "date"s;
static const std::string time_name      = "time"s;
static const std::string smallint_name  = "smallint"s;
static const std::string tinyint_name   = "tinyint"s;
static const std::string duration_name  = "duration"s;
static const std::string list_name      = "list"s;
static const std::string map_name       = "map"s;
static const std::string set_name       = "set"s;
static const std::string udt_name       = "udt"s;
static const std::string tuple_name     = "tuple"s;
static const std::string unknown_name   = "unknown"s;

auto to_string(data_type type) -> const std::string&
{
    switch (type)
    {
        case data_type::unknown:
            return unknown_name;
        case data_type::custom:
            return custom_name;
        case data_type::ascii:
            return ascii_name;
        case data_type::bigint:
            return bigint_name;
        case data_type::blob:
            return blob_name;
        case data_type::boolean:
            return boolean_name;
        case data_type::counter:
            return counter_name;
        case data_type::decimal:
            return decimal_name;
        case data_type::double_t:
            return double_name;
        case data_type::float_t:
            return float_name;
        case data_type::int_t:
            return int_name;
        case data_type::text:
            return text_name;
        case data_type::timestamp:
            return timestamp_name;
        case data_type::uuid:
            return uuid_name;
        case data_type::varchar:
            return varchar_name;
        case data_type::varint:
            return varint_name;
        case data_type::timeuuid:
            return timeuuid_name;
        case data_type::inet:
            return inet_name;
        case data_type::date:
            return date_name;
        case data_type::time:
            return time_name;
        case data_type::smallint:
            return smallint_name;
        case data_type::tinyint:
            return tinyint_name;
        case data_type::duration:
            return duration_name;
        case data_type::list:
            return list_name;
        case data_type::map:
            return map_name;
        case data_type::set:
            return set_name;
        case data_type::udt:
            return udt_name;
        case data_type::tuple:
            return tuple_name;
    }

    return unknown_name;
}

} // namespace priam
