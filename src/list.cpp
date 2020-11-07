#include "priam/list.hpp"

namespace priam
{
statement_list::statement_list(size_t reserve_size)
    : m_cass_collection_ptr(cass_collection_new(CassCollectionType::CASS_COLLECTION_TYPE_LIST, reserve_size))
{
}

auto statement_list::append_ascii(std::string_view data) -> bool
{
    return cass_collection_append_string_n(m_cass_collection_ptr.get(), data.data(), data.length()) == CASS_OK;
}

auto statement_list::append_big_int(int64_t value) -> bool
{
    return cass_collection_append_int64(m_cass_collection_ptr.get(), value) == CASS_OK;
}

auto statement_list::append_blob(blob blob) -> bool
{
    return cass_collection_append_bytes(
               m_cass_collection_ptr.get(), reinterpret_cast<const cass_byte_t*>(blob.data()), blob.size()) == CASS_OK;
}

auto statement_list::append_boolean(bool value) -> bool
{
    return cass_collection_append_bool(m_cass_collection_ptr.get(), static_cast<cass_bool_t>(value)) == CASS_OK;
}

auto statement_list::append_counter(int64_t value) -> bool
{
    return cass_collection_append_int64(m_cass_collection_ptr.get(), value) == CASS_OK;
}

auto statement_list::append_decimal(decimal value) -> bool
{
    const auto& varint = value.variable_int();
    return cass_collection_append_decimal(
               m_cass_collection_ptr.get(),
               reinterpret_cast<ptr<const cass_byte_t>>(varint.data()),
               varint.size(),
               value.scale()) == CASS_OK;
}

auto statement_list::append_double(double value) -> bool
{
    return cass_collection_append_double(m_cass_collection_ptr.get(), value) == CASS_OK;
}

auto statement_list::append_float(float value) -> bool
{
    return cass_collection_append_float(m_cass_collection_ptr.get(), value) == CASS_OK;
}

auto statement_list::append_int(int32_t value) -> bool
{
    return cass_collection_append_int32(m_cass_collection_ptr.get(), value) == CASS_OK;
}

auto statement_list::append_text(std::string_view data) -> bool
{
    return append_ascii(data);
}

auto statement_list::append_timestamp(std::time_t timestamp) -> bool
{
    return cass_collection_append_uint32(m_cass_collection_ptr.get(), static_cast<cass_uint32_t>(timestamp)) == CASS_OK;
}

auto statement_list::append_uuid(std::string_view uuid) -> bool
{
    CassUuid  cass_uuid;
    CassError rc = cass_uuid_from_string_n(uuid.data(), uuid.length(), &cass_uuid);
    if (rc != CASS_OK)
    {
        return false;
    }
    return cass_collection_append_uuid(m_cass_collection_ptr.get(), cass_uuid) == CASS_OK;
}

auto statement_list::append_varchar(std::string_view data) -> bool
{
    return append_ascii(data);
}

auto statement_list::append_time_uuid(std::string_view timeuuid) -> bool
{
    return append_uuid(timeuuid);
}

auto statement_list::append_inet(std::string_view inet) -> bool
{
    CassInet  cass_inet;
    CassError rc = cass_inet_from_string_n(inet.data(), inet.length(), &cass_inet);
    if (rc != CASS_OK)
    {
        return false;
    }
    return cass_collection_append_inet(m_cass_collection_ptr.get(), cass_inet) == CASS_OK;
}

auto statement_list::append_date(uint32_t date) -> bool
{
    return cass_collection_append_uint32(m_cass_collection_ptr.get(), date) == CASS_OK;
}

auto statement_list::append_time(int64_t time) -> bool
{
    return cass_collection_append_int64(m_cass_collection_ptr.get(), time) == CASS_OK;
}

auto statement_list::append_tiny_int(int8_t value) -> bool
{
    return cass_collection_append_int8(m_cass_collection_ptr.get(), value) == CASS_OK;
}

auto statement_list::append_duration(duration duration) -> bool
{
    return cass_collection_append_duration(
               m_cass_collection_ptr.get(), duration.months(), duration.days(), duration.nanos()) == CASS_OK;
}

auto statement_list::append_list(statement_list list) -> bool
{
    return cass_collection_append_collection(m_cass_collection_ptr.get(), list.m_cass_collection_ptr.get()) == CASS_OK;
}

result_list::result_list(const CassValue* cass_value) : m_cass_value(cass_value)
{
}

} // namespace priam
