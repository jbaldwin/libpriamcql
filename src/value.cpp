#include "priam/value.hpp"
#include "priam/list.hpp"
#include "priam/map.hpp"
#include "priam/set.hpp"
#include "priam/tuple.hpp"

#include <ctime>
#include <string>

namespace priam
{
auto value::is_null() const -> bool
{
    return static_cast<bool>(cass_value_is_null(m_cass_value));
}

auto value::type() const -> CassValueType
{
    const CassDataType* cass_data_type = cass_value_data_type(m_cass_value);
    if (cass_data_type != nullptr)
    {
        return cass_data_type_type(cass_data_type);
    }

    return CassValueType::CASS_VALUE_TYPE_UNKNOWN;
}

auto value::as_ascii() const -> std::optional<std::string>
{
    if (!is_null())
    {
        const char* output{nullptr};
        size_t      output_len{0};
        if (cass_value_get_string(m_cass_value, &output, &output_len) == CASS_OK)
        {
            return {std::string(output, output_len)};
        }
    }
    return std::nullopt;
}

auto value::as_big_int() const -> std::optional<int64_t>
{
    if (!is_null())
    {
        int64_t output{0};
        if (cass_value_get_int64(m_cass_value, &output) == CASS_OK)
        {
            return {output};
        }
    }
    return std::nullopt;
}

auto value::as_blob() const -> std::optional<blob>
{
    if (!is_null())
    {
        ptr<const cass_byte_t> bytes{nullptr};
        size_t                 bytes_size{0};
        if (cass_value_get_bytes(m_cass_value, &bytes, &bytes_size) == CASS_OK)
        {
            return {blob{reinterpret_cast<ptr<const std::byte>>(bytes), bytes_size}};
        }
    }
    return std::nullopt;
}

auto value::as_boolean() const -> std::optional<bool>
{
    if (!is_null())
    {
        cass_bool_t output{cass_false};
        if (cass_value_get_bool(m_cass_value, &output) == CASS_OK)
        {
            return {static_cast<bool>(output)};
        }
    }
    return std::nullopt;
}

auto value::as_counter() const -> std::optional<int64_t>
{
    if (!is_null())
    {
        int64_t output{0};
        if (cass_value_get_int64(m_cass_value, &output) == CASS_OK)
        {
            return {output};
        }
    }
    return std::nullopt;
}

auto value::as_decimal() const -> std::optional<decimal>
{
    if (!is_null())
    {
        ptr<const cass_byte_t> varint{nullptr};
        size_t                 varint_size{0};
        cass_int32_t           scale{0};
        if (cass_value_get_decimal(m_cass_value, &varint, &varint_size, &scale) == CASS_OK)
        {
            return {decimal{blob{reinterpret_cast<ptr<const std::byte>>(varint), varint_size}, scale}};
        }
    }
    return std::nullopt;
}

auto value::as_double() const -> std::optional<double>
{
    if (!is_null())
    {
        double output{0.0};
        if (cass_value_get_double(m_cass_value, &output) == CASS_OK)
        {
            return {output};
        }
    }
    return std::nullopt;
}

auto value::as_float() const -> std::optional<float>
{
    if (!is_null())
    {
        float output{0.0f};
        if (cass_value_get_float(m_cass_value, &output) == CASS_OK)
        {
            return {output};
        }
    }
    return std::nullopt;
}

auto value::as_int() const -> std::optional<int32_t>
{
    if (!is_null())
    {
        int32_t output{0};
        if (cass_value_get_int32(m_cass_value, &output) == CASS_OK)
        {
            return {output};
        }
    }
    return std::nullopt;
}

auto value::as_text() const -> std::optional<std::string>
{
    return as_ascii();
}

auto value::as_timestamp() const -> std::optional<std::time_t>
{
    if (!is_null())
    {
        cass_uint32_t year_month_day{0};
        if (cass_value_get_uint32(m_cass_value, &year_month_day) == CASS_OK)
        {
            return {static_cast<std::time_t>(year_month_day)};
        }
    }
    return std::nullopt;
}

auto value::as_timestamp_date_formatted() const -> std::optional<std::string>
{
    auto timestamp_opt = as_timestamp();
    if (timestamp_opt.has_value())
    {
        char        buffer[26];
        std::string output{ctime_r(&timestamp_opt.value(), buffer)};
        return {std::move(output)};
    }
    return std::nullopt;
}

auto value::as_uuid() const -> std::optional<uuid>
{
    if (!is_null())
    {
        CassUuid uuid;
        if (cass_value_get_uuid(m_cass_value, &uuid) == CASS_OK)
        {
            return {uuid};
        }
    }
    return std::nullopt;
}

auto value::as_varchar() const -> std::optional<std::string>
{
    return as_ascii();
}

auto value::as_varint() const -> std::optional<blob>
{
    return as_blob();
}

auto value::as_time_uuid() const -> std::optional<uuid>
{
    return as_uuid();
}

auto value::as_inet() const -> std::optional<std::string>
{
    if (!is_null())
    {
        // TODO make this function return the struct and provide a priam::to_string()
        CassInet cass_inet;
        if (cass_value_get_inet(m_cass_value, &cass_inet) == CASS_OK)
        {
            std::string output(CASS_INET_STRING_LENGTH, '\0');
            cass_inet_string(cass_inet, output.data());

            auto pos = output.find('\0');
            if (pos != std::string::npos)
            {
                output.resize(pos);
            }
            return {std::move(output)};
        }
    }
    return std::nullopt;
}

auto value::as_date() const -> std::optional<uint32_t>
{
    if (!is_null())
    {
        uint32_t output{0};
        if (cass_value_get_uint32(m_cass_value, &output) == CASS_OK)
        {
            return {output};
        }
    }
    return std::nullopt;
}

auto value::as_time() const -> std::optional<int64_t>
{
    if (!is_null())
    {
        int64_t output{0};
        if (cass_value_get_int64(m_cass_value, &output) == CASS_OK)
        {
            return {output};
        }
    }
    return std::nullopt;
}

auto value::as_small_int() const -> std::optional<int16_t>
{
    if (!is_null())
    {
        int16_t output{0};
        if (cass_value_get_int16(m_cass_value, &output) == CASS_OK)
        {
            return output;
        }
    }
    return std::nullopt;
}

auto value::as_tiny_int() const -> std::optional<int8_t>
{
    if (!is_null())
    {
        int8_t output{0};
        if (cass_value_get_int8(m_cass_value, &output) == CASS_OK)
        {
            return {output};
        }
    }
    return std::nullopt;
}

auto value::as_duration() const -> std::optional<duration>
{
    if (!is_null())
    {
        cass_int32_t months{0};
        cass_int32_t days{0};
        cass_int64_t nanos{0};
        if (cass_value_get_duration(m_cass_value, &months, &days, &nanos) == CASS_OK)
        {
            return {duration{months, days, nanos}};
        }
    }
    return std::nullopt;
}

auto value::as_list() const -> std::optional<priam::result_list>
{
    if (!is_null())
    {
        return {result_list{m_cass_value}};
    }
    return std::nullopt;
}

auto value::as_map() const -> std::optional<priam::map>
{
    if (!is_null())
    {
        return {map{m_cass_value}};
    }
    return std::nullopt;
}

auto value::as_set() const -> std::optional<priam::set>
{
    if (!is_null())
    {
        return {set{m_cass_value}};
    }
    return std::nullopt;
}

auto value::as_tuple() const -> std::optional<priam::tuple>
{
    if (!is_null())
    {
        return {tuple{m_cass_value}};
    }
    return std::nullopt;
}

value::value(const CassValue* cass_column) : m_cass_value(cass_column)
{
}

} // namespace priam
