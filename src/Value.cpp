#include "priam/Value.hpp"
#include "priam/Set.hpp"
#include "priam/Tuple.hpp"
#include "priam/list.hpp"
#include "priam/map.hpp"

#include <ctime>
#include <string>

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

auto Value::IsNull() const -> bool
{
    return static_cast<bool>(cass_value_is_null(m_cass_value));
}

auto Value::DataType() const -> CassValueType
{
    const CassDataType* cass_data_type = cass_value_data_type(m_cass_value);
    if (cass_data_type != nullptr)
    {
        return cass_data_type_type(cass_data_type);
    }

    return CassValueType::CASS_VALUE_TYPE_UNKNOWN;
}

auto Value::AsASCII() const -> std::string
{
    const char* output{nullptr};
    size_t      output_len{0};
    if (cass_value_get_string(m_cass_value, &output, &output_len) == CASS_OK)
    {
        if (output != nullptr && output_len > 0)
        {
            return std::string(output, output_len);
        }
    }
    return std::string{};
}

auto Value::AsBigInt() const -> int64_t
{
    int64_t output{0};
    cass_value_get_int64(m_cass_value, &output);
    return output;
}

auto Value::AsBlob() const -> blob
{
    ptr<const cass_byte_t> bytes{nullptr};
    size_t                 bytes_size{0};
    if (cass_value_get_bytes(m_cass_value, &bytes, &bytes_size) == CASS_OK)
    {
        if (bytes != nullptr && bytes_size > 0)
        {
            return blob(reinterpret_cast<ptr<const std::byte>>(bytes), bytes_size);
        }
    }
    return blob(nullptr, 0);
}

auto Value::AsBoolean() const -> bool
{
    cass_bool_t output{cass_false};
    cass_value_get_bool(m_cass_value, &output);
    return static_cast<bool>(output);
}

auto Value::AsCounter() const -> int64_t
{
    int64_t output{0};
    cass_value_get_int64(m_cass_value, &output);
    return output;
}

auto Value::AsDecimal() const -> decimal
{
    ptr<const cass_byte_t> varint{nullptr};
    size_t                 varint_size{0};
    cass_int32_t           scale{0};
    cass_value_get_decimal(m_cass_value, &varint, &varint_size, &scale);
    return decimal(blob(reinterpret_cast<ptr<const std::byte>>(varint), varint_size), scale);
}

auto Value::AsDouble() const -> double
{
    double output{0.0};
    cass_value_get_double(m_cass_value, &output);
    return output;
}

auto Value::AsFloat() const -> float
{
    float output{0.0f};
    cass_value_get_float(m_cass_value, &output);
    return output;
}

auto Value::AsInt() const -> int32_t
{
    int32_t output{0};
    cass_value_get_int32(m_cass_value, &output);
    return output;
}

auto Value::AsText() const -> std::string
{
    return AsASCII();
}

auto Value::AsTimestamp() const -> std::time_t
{
    cass_uint32_t year_month_day{0};
    cass_value_get_uint32(m_cass_value, &year_month_day);
    return static_cast<std::time_t>(year_month_day);
}

auto Value::AsTimestampAsDateFormatted() const -> std::string
{
    cass_uint32_t year_month_day{0};
    cass_value_get_uint32(m_cass_value, &year_month_day);
    auto time = static_cast<std::time_t>(year_month_day);

    std::string output(64, '\0');
    ctime_r(&time, output.data());

    return output;
}

auto Value::AsUUID() const -> std::string
{
    CassUuid uuid;
    if (cass_value_get_uuid(m_cass_value, &uuid) == CASS_OK)
    {
        std::string output(CASS_UUID_STRING_LENGTH, '\0');
        cass_uuid_string(uuid, output.data());
        return output.substr(0, (CASS_UUID_STRING_LENGTH - 1));
    }
    return std::string{};
}

auto Value::AsVarChar() const -> std::string
{
    return AsASCII();
}

auto Value::AsVarInt() const -> blob
{
    return AsBlob();
}

auto Value::AsTimeUUID() const -> std::string
{
    return AsUUID();
}

auto Value::AsINet() const -> std::string
{
    CassInet cass_inet;
    cass_value_get_inet(m_cass_value, &cass_inet);
    char* output{nullptr};
    cass_inet_string(cass_inet, output);
    return std::string{output};
}

auto Value::AsDate() const -> uint32_t
{
    uint32_t output{0};
    cass_value_get_uint32(m_cass_value, &output);
    return output;
}

auto Value::AsTime() const -> int64_t
{
    int64_t output{0};
    cass_value_get_int64(m_cass_value, &output);
    return output;
}

auto Value::AsSmallInt() const -> int16_t
{
    int16_t output{0};
    cass_value_get_int16(m_cass_value, &output);
    return output;
}

auto Value::AsTinyInt() const -> int8_t
{
    int8_t output{0};
    cass_value_get_int8(m_cass_value, &output);
    return output;
}

auto Value::AsDuration() const -> duration
{
    cass_int32_t months{0};
    cass_int32_t days{0};
    cass_int64_t nanos{0};
    cass_value_get_duration(m_cass_value, &months, &days, &nanos);
    return duration{months, days, nanos};
}

auto Value::AsList() const -> priam::result_list
{
    return result_list(m_cass_value);
}

auto Value::AsMap() const -> priam::map
{
    return map(m_cass_value);
}

auto Value::AsSet() const -> priam::Set
{
    return Set(m_cass_value);
}

auto Value::AsTuple() const -> priam::Tuple
{
    return Tuple(m_cass_value);
}

Value::Value(const CassValue* cass_column) : m_cass_value(cass_column)
{
}

} // namespace priam
