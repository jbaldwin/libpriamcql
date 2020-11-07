#include "priam/type.hpp"

namespace priam
{
static const std::string CUSTOM     = "CUSTOM";
static const std::string ASCII      = "ASCII";
static const std::string BIGINT     = "BIGINT";
static const std::string BLOB       = "BLOB";
static const std::string BOOLEAN    = "BOOLEAN";
static const std::string COUNTER    = "COUNTER";
static const std::string DECIMAL    = "DECIMAL";
static const std::string DOUBLE     = "DOUBLE";
static const std::string FLOAT      = "FLOAT";
static const std::string INT        = "INT";
static const std::string TEXT       = "TEXT";
static const std::string TIMESTAMP  = "TIMESTAMP";
static const std::string UUID       = "UUID";
static const std::string VARCHAR    = "VARCHAR";
static const std::string VARINT     = "VARINT";
static const std::string TIMEUUID   = "TIMEUUID";
static const std::string INET       = "INET";
static const std::string DATE       = "DATE";
static const std::string TIME       = "TIME";
static const std::string SMALL_INT  = "SMALL_INT";
static const std::string TINY_INT   = "TINY_INT";
static const std::string DURATION   = "DURATION";
static const std::string LIST       = "LIST";
static const std::string MAP        = "MAP";
static const std::string SET        = "SET";
static const std::string UDT        = "UDT";
static const std::string TUPLE      = "TUPLE";
static const std::string LAST_ENTRY = "LAST_ENTRY";
static const std::string UNKNOWN    = "UNKNOWN";

auto to_string(CassValueType type) -> const std::string&
{
    switch (type)
    {
        case CASS_VALUE_TYPE_CUSTOM:
            return CUSTOM;
        case CASS_VALUE_TYPE_ASCII:
            return ASCII;
        case CASS_VALUE_TYPE_BIGINT:
            return BIGINT;
        case CASS_VALUE_TYPE_BLOB:
            return BLOB;
        case CASS_VALUE_TYPE_BOOLEAN:
            return BOOLEAN;
        case CASS_VALUE_TYPE_COUNTER:
            return COUNTER;
        case CASS_VALUE_TYPE_DECIMAL:
            return DECIMAL;
        case CASS_VALUE_TYPE_DOUBLE:
            return DOUBLE;
        case CASS_VALUE_TYPE_FLOAT:
            return FLOAT;
        case CASS_VALUE_TYPE_INT:
            return INT;
        case CASS_VALUE_TYPE_TEXT:
            return TEXT;
        case CASS_VALUE_TYPE_TIMESTAMP:
            return TIMESTAMP;
        case CASS_VALUE_TYPE_UUID:
            return UUID;
        case CASS_VALUE_TYPE_VARCHAR:
            return VARCHAR;
        case CASS_VALUE_TYPE_VARINT:
            return VARINT;
        case CASS_VALUE_TYPE_TIMEUUID:
            return TIMEUUID;
        case CASS_VALUE_TYPE_INET:
            return INET;
        case CASS_VALUE_TYPE_DATE:
            return DATE;
        case CASS_VALUE_TYPE_TIME:
            return TIME;
        case CASS_VALUE_TYPE_SMALL_INT:
            return SMALL_INT;
        case CASS_VALUE_TYPE_TINY_INT:
            return TINY_INT;
        case CASS_VALUE_TYPE_DURATION:
            return DURATION;
        case CASS_VALUE_TYPE_LIST:
            return LIST;
        case CASS_VALUE_TYPE_MAP:
            return MAP;
        case CASS_VALUE_TYPE_SET:
            return SET;
        case CASS_VALUE_TYPE_UDT:
            return UDT;
        case CASS_VALUE_TYPE_TUPLE:
            return TUPLE;
        case CASS_VALUE_TYPE_LAST_ENTRY:
            return LAST_ENTRY;
        case CASS_VALUE_TYPE_UNKNOWN:
            return UNKNOWN;
    }

    return UNKNOWN; // gcc requires this even though its dead code
}

} // namespace priam
