#include "vidar/Column.h"

#include <ctime>

namespace vidar
{

static const std::string CUSTOM = "CUSTOM";
static const std::string ASCII = "ASCII";
static const std::string BIGINT = "BIGINT";
static const std::string BLOB = "BLOB";
static const std::string BOOLEAN = "BOOLEAN";
static const std::string COUNTER = "COUNTER";
static const std::string DECIMAL = "DECIMAL";
static const std::string DOUBLE  = "DOUBLE";
static const std::string FLOAT = "FLOAT";
static const std::string INT = "INT";
static const std::string TEXT = "TEXT";
static const std::string TIMESTAMP = "TIMESTAMP";
static const std::string UUID = "UUID";
static const std::string VARCHAR = "VARCHAR";
static const std::string VARINT = "VARINT";
static const std::string TIMEUUID = "TIMEUUID";
static const std::string INET = "INET";
static const std::string DATE = "DATE";
static const std::string TIME = "TIME";
static const std::string SMALL_INT = "SMALL_INT";
static const std::string TINY_INT = "TINY_INT";
static const std::string DURATION = "DURATION";
static const std::string LIST = "LIST";
static const std::string MAP = "MAP";
static const std::string SET = "SET";
static const std::string UDT = "UDT";
static const std::string TUPLE = "TUPLE";
static const std::string LAST_ENTRY = "LAST_ENTRY";
static const std::string UNKNOWN = "UNKNOWN";


auto to_string(CassValueType type) -> const std::string&
{
    switch(type)
    {
        case CASS_VALUE_TYPE_CUSTOM: return CUSTOM;
        case CASS_VALUE_TYPE_ASCII: return ASCII;
        case CASS_VALUE_TYPE_BIGINT: return BIGINT;
        case CASS_VALUE_TYPE_BLOB: return BLOB;
        case CASS_VALUE_TYPE_BOOLEAN: return BOOLEAN;
        case CASS_VALUE_TYPE_COUNTER: return COUNTER;
        case CASS_VALUE_TYPE_DECIMAL: return DECIMAL;
        case CASS_VALUE_TYPE_DOUBLE: return DOUBLE;
        case CASS_VALUE_TYPE_FLOAT: return FLOAT;
        case CASS_VALUE_TYPE_INT: return INT;
        case CASS_VALUE_TYPE_TEXT: return TEXT;
        case CASS_VALUE_TYPE_TIMESTAMP: return TIMESTAMP;
        case CASS_VALUE_TYPE_UUID: return UUID;
        case CASS_VALUE_TYPE_VARCHAR: return VARCHAR;
        case CASS_VALUE_TYPE_VARINT: return VARINT;
        case CASS_VALUE_TYPE_TIMEUUID: return TIMEUUID;
        case CASS_VALUE_TYPE_INET: return INET;
        case CASS_VALUE_TYPE_DATE: return DATE;
        case CASS_VALUE_TYPE_TIME: return TIME;
        case CASS_VALUE_TYPE_SMALL_INT: return SMALL_INT;
        case CASS_VALUE_TYPE_TINY_INT: return TINY_INT;
        case CASS_VALUE_TYPE_DURATION: return DURATION;
        case CASS_VALUE_TYPE_LIST: return LIST;
        case CASS_VALUE_TYPE_MAP: return MAP;
        case CASS_VALUE_TYPE_SET: return SET;
        case CASS_VALUE_TYPE_UDT: return UDT;
        case CASS_VALUE_TYPE_TUPLE: return TUPLE;
        case CASS_VALUE_TYPE_LAST_ENTRY: return LAST_ENTRY;
        case CASS_VALUE_TYPE_UNKNOWN: return UNKNOWN;
    }
}

auto Column::GetDataType() const -> CassValueType
{
    const CassDataType* cass_data_type = cass_value_data_type(m_cass_column);
    if(cass_data_type != nullptr)
    {
        return cass_data_type_type(cass_data_type);
    }

    return CassValueType::CASS_VALUE_TYPE_UNKNOWN;
}

auto Column::GetASCII() const -> std::string
{
    const char* output;
    size_t output_len;
    cass_value_get_string(m_cass_column, &output, &output_len);
    return std::string(output, output_len);
}

auto Column::GetTimestamp() const -> std::time_t
{
    cass_uint32_t year_month_day;
    cass_value_get_uint32(m_cass_column, &year_month_day);
    return static_cast<std::time_t>(year_month_day);
}

auto Column::GetTimestampAsDateFormatted() const -> std::string
{
    cass_uint32_t year_month_day;
    cass_value_get_uint32(m_cass_column, &year_month_day);
    auto time = static_cast<std::time_t>(year_month_day);

    std::string output(64, '\0');
    ctime_r(&time, output.data());

    return output;
}

//auto Column::GetInt32() const -> int32_t
//{
//    int32_t output = 0;
//    cass_value_get_int32(m_cass_column, &output);
//    return output;
//}
//
//auto Column::GetUInt32() const -> uint32_t
//{
//    uint32_t output = 0;
//    cass_value_get_uint32(m_cass_column, &output);
//    return output;
//}

Column::Column(
    const CassValue* cass_column
)
    : m_cass_column(cass_column)
{

}

} // namespace vidar
