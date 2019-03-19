#include "priam/List.h"

namespace priam
{

StatementList::StatementList(
    size_t item_count
)
    : m_cass_collection_ptr(cass_collection_new(CassCollectionType::CASS_COLLECTION_TYPE_LIST, item_count))
{

}

auto StatementList::AppendASCII(
    std::string_view data
) -> bool
{
    return cass_collection_append_string_n(m_cass_collection_ptr.get(), data.data(), data.length()) == CASS_OK;
}

auto StatementList::AppendBigInt(
    int64_t value
) -> bool
{
    return cass_collection_append_int64(m_cass_collection_ptr.get(), value) == CASS_OK;
}

auto StatementList::AppendBlob(
    Blob blob
) -> bool
{
    return cass_collection_append_bytes(
        m_cass_collection_ptr.get(),
        reinterpret_cast<const cass_byte_t*>(blob.GetBytes()),
        blob.GetLenth()
    ) == CASS_OK;
}

auto StatementList::AppendBoolean(
    bool value
) -> bool
{
    return cass_collection_append_bool(m_cass_collection_ptr.get(), static_cast<cass_bool_t>(value)) == CASS_OK;
}

auto StatementList::AppendCounter(
    int64_t value
) -> bool
{
    return cass_collection_append_int64(m_cass_collection_ptr.get(), value) == CASS_OK;
}

auto StatementList::AppendDecimal(
    Decimal value
) -> bool
{
    const auto& varint = value.GetVariableInt();
    return cass_collection_append_decimal(
        m_cass_collection_ptr.get(),
        reinterpret_cast<const cass_byte_t*>(varint.GetBytes()),
        varint.GetLenth(),
        value.GetScale()
    ) == CASS_OK;
}

auto StatementList::AppendDouble(
    double value
) -> bool
{
    return cass_collection_append_double(m_cass_collection_ptr.get(), value) == CASS_OK;
}

auto StatementList::AppendFloat(
    float value
) -> bool
{
    return cass_collection_append_float(m_cass_collection_ptr.get(),  value) == CASS_OK;
}

auto StatementList::AppendInt(
    int32_t value
) -> bool
{
    return cass_collection_append_int32(m_cass_collection_ptr.get(),  value) == CASS_OK;
}

auto StatementList::AppendText(
    std::string_view data
) -> bool
{
    return AppendASCII(data);
}

auto StatementList::AppendTimestamp(
    std::time_t timestamp
) -> bool
{
    return cass_collection_append_uint32(m_cass_collection_ptr.get(), static_cast<cass_uint32_t>(timestamp)) == CASS_OK;
}

auto StatementList::AppendUUID(
    std::string_view uuid
) -> bool
{
    CassUuid cass_uuid;
    CassError rc = cass_uuid_from_string_n(uuid.data(), uuid.length(), &cass_uuid);
    if(rc != CASS_OK)
    {
        return false;
    }
    return cass_collection_append_uuid(m_cass_collection_ptr.get(), cass_uuid) == CASS_OK;
}

auto StatementList::AppendVarChar(
    std::string_view data
) -> bool
{
    return AppendASCII(data);
}

auto StatementList::AppendTimeUUID(
    std::string_view timeuuid
) -> bool
{
    return AppendUUID(timeuuid);
}

auto StatementList::AppendINet(
    std::string_view inet
) -> bool
{
    CassInet cass_inet;
    CassError rc = cass_inet_from_string_n(inet.data(), inet.length(), &cass_inet);
    if(rc != CASS_OK)
    {
        return false;
    }
    return cass_collection_append_inet(m_cass_collection_ptr.get(), cass_inet) == CASS_OK;
}

auto StatementList::AppendDate(
    uint32_t date
) -> bool
{
    return cass_collection_append_uint32(m_cass_collection_ptr.get(), date) == CASS_OK;
}

auto StatementList::AppendTime(
    int64_t time
) -> bool
{
    return cass_collection_append_int64(m_cass_collection_ptr.get(), time) == CASS_OK;
}

auto StatementList::AppendTinyInt(
    int8_t value
) -> bool
{
    return cass_collection_append_int8(m_cass_collection_ptr.get(), value) == CASS_OK;
}

auto StatementList::AppendDuration(
    Duration duration
) -> bool
{
    return cass_collection_append_duration(m_cass_collection_ptr.get(), duration.GetMonths(), duration.GetDays(), duration.GetNanos()) == CASS_OK;
}

auto StatementList::AppendList(
    StatementList list
) -> bool
{
    return  cass_collection_append_collection(m_cass_collection_ptr.get(), list.m_cass_collection_ptr.get()) == CASS_OK;
}

ResultList::ResultList(
    const CassValue* cass_value
)
    : m_cass_value(cass_value)
{

}

} // priam
