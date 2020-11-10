#include "priam/statement.hpp"

#include <algorithm>

namespace priam
{
statement::statement(std::string_view query)
    : m_parameter_count(std::count(query.begin(), query.end(), '?')),
      m_cass_statement_ptr(cass_statement_new_n(query.data(), query.length(), m_parameter_count))
{
}

// statement::statement(statement&& other)
//     : m_parameter_count(std::exchange(other.m_parameter_count, 0)),
//       m_cass_statement_ptr(std::move(other.m_cass_statement_ptr))
// {

// }

// auto statement::operator=(statement&& other) -> statement&
// {
//     if(std::addressof(other) != this)
//     {
//         m_parameter_count = std::exchange(other.m_parameter_count, 0);
//         m_cass_statement_ptr = std::move(other.m_cass_statement_ptr);
//     }
//     return *this;
// }

auto statement::bind_null(size_t position) -> status
{
    return static_cast<status>(cass_statement_bind_null(m_cass_statement_ptr.get(), position));
}

auto statement::bind_null(std::string_view name) -> status
{
    return static_cast<status>(
        cass_statement_bind_null_by_name_n(m_cass_statement_ptr.get(), name.data(), name.length()));
}

auto statement::bind_boolean(bool value, size_t position) -> status
{
    return static_cast<status>(
        cass_statement_bind_bool(m_cass_statement_ptr.get(), position, static_cast<cass_bool_t>(value)));
}

auto statement::bind_boolean(bool value, std::string_view name) -> status
{
    return static_cast<status>(cass_statement_bind_bool_by_name_n(
        m_cass_statement_ptr.get(), name.data(), name.length(), static_cast<cass_bool_t>(value)));
}

auto statement::bind_uuid(uuid uuid, size_t position) -> status
{
    return static_cast<status>(cass_statement_bind_uuid(m_cass_statement_ptr.get(), position, uuid));
}

auto statement::bind_uuid(uuid uuid, std::string_view name) -> status
{
    return static_cast<status>(
        cass_statement_bind_uuid_by_name_n(m_cass_statement_ptr.get(), name.data(), name.length(), uuid));
}

auto statement::bind_uuid(std::string_view uuid, size_t position) -> status
{
    CassUuid cass_uuid{};
    // Cass lib seems very particular about the length always being "36".
    size_t    length = (uuid.length() >= CASS_UUID_STRING_LENGTH) ? 36 : uuid.length();
    CassError rc     = cass_uuid_from_string_n(uuid.data(), length, &cass_uuid);
    if (rc != CASS_OK)
    {
        return static_cast<status>(rc);
    }
    return static_cast<status>(cass_statement_bind_uuid(m_cass_statement_ptr.get(), position, cass_uuid));
}

auto statement::bind_uuid(std::string_view uuid, std::string_view name) -> status
{
    CassUuid  cass_uuid{};
    size_t    length = (uuid.length() >= CASS_UUID_STRING_LENGTH) ? 36 : uuid.length();
    CassError rc     = cass_uuid_from_string_n(uuid.data(), length, &cass_uuid);
    if (rc != CASS_OK)
    {
        return static_cast<status>(rc);
    }
    return static_cast<status>(
        cass_statement_bind_uuid_by_name_n(m_cass_statement_ptr.get(), name.data(), name.length(), cass_uuid));
}

auto statement::bind_text(std::string_view data, size_t position) -> status
{
    return static_cast<status>(
        cass_statement_bind_string_n(m_cass_statement_ptr.get(), position, data.data(), data.length()));
}

auto statement::bind_text(std::string_view data, std::string_view name) -> status
{
    return static_cast<status>(cass_statement_bind_string_by_name_n(
        m_cass_statement_ptr.get(), name.data(), name.length(), data.data(), data.length()));
}

auto statement::bind_tiny_int(int8_t value, size_t position) -> status
{
    return static_cast<status>(cass_statement_bind_int8(m_cass_statement_ptr.get(), position, value));
}

auto statement::bind_tiny_int(int8_t value, std::string_view name) -> status
{
    return static_cast<status>(
        cass_statement_bind_int8_by_name_n(m_cass_statement_ptr.get(), name.data(), name.length(), value));
}

auto statement::bind_int(int32_t value, size_t position) -> status
{
    return static_cast<status>(cass_statement_bind_int32(m_cass_statement_ptr.get(), position, value));
}

auto statement::bind_int(int32_t value, std::string_view name) -> status
{
    return static_cast<status>(
        cass_statement_bind_int32_by_name_n(m_cass_statement_ptr.get(), name.data(), name.length(), value));
}

auto statement::bind_big_int(int64_t value, size_t position) -> status
{
    return static_cast<status>(cass_statement_bind_int64(m_cass_statement_ptr.get(), position, value));
}

auto statement::bind_big_int(int64_t value, std::string_view name) -> status
{
    return static_cast<status>(
        cass_statement_bind_int64_by_name_n(m_cass_statement_ptr.get(), name.data(), name.length(), value));
}

auto statement::bind_float(float value, size_t position) -> status
{
    return static_cast<status>(cass_statement_bind_float(m_cass_statement_ptr.get(), position, value));
}

auto statement::bind_float(float value, std::string_view name) -> status
{
    return static_cast<status>(
        cass_statement_bind_float_by_name_n(m_cass_statement_ptr.get(), name.data(), name.length(), value));
}

auto statement::bind_double(double value, size_t position) -> status
{
    return static_cast<status>(cass_statement_bind_double(m_cass_statement_ptr.get(), position, value));
}

auto statement::bind_double(double value, std::string_view name) -> status
{
    return static_cast<status>(
        cass_statement_bind_double_by_name_n(m_cass_statement_ptr.get(), name.data(), name.length(), value));
}

auto statement::bind_list(statement_list list, size_t position) -> status
{
    return static_cast<status>(
        cass_statement_bind_collection(m_cass_statement_ptr.get(), position, list.m_cass_collection_ptr.get()));
}

auto statement::bind_list(statement_list list, std::string_view name) -> status
{
    return static_cast<status>(cass_statement_bind_collection_by_name_n(
        m_cass_statement_ptr.get(), name.data(), name.length(), list.m_cass_collection_ptr.get()));
}

auto statement::bind_blob(blob blob, size_t position) -> status
{
    return static_cast<status>(cass_statement_bind_bytes(
        m_cass_statement_ptr.get(), position, reinterpret_cast<ptr<const cass_uint8_t>>(blob.data()), blob.size()));
}

auto statement::bind_blob(blob blob, std::string_view name) -> status
{
    return static_cast<status>(cass_statement_bind_bytes_by_name_n(
        m_cass_statement_ptr.get(),
        name.data(),
        name.length(),
        reinterpret_cast<ptr<const cass_uint8_t>>(blob.data()),
        blob.size()));
}

auto statement::reset() -> status
{
    return static_cast<status>(cass_statement_reset_parameters(m_cass_statement_ptr.get(), m_parameter_count));
}

statement::statement(const CassPrepared* cass_prepared, size_t parameter_count)
    : m_parameter_count(parameter_count),
      m_cass_statement_ptr(cass_prepared_bind(cass_prepared))
{
}

} // namespace priam
