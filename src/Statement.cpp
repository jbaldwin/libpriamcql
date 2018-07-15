#include "priam/Statement.h"

namespace priam
{

auto Statement::BindNull(
    size_t position
) -> bool
{
    CassError rc = cass_statement_bind_null(m_cass_statement_ptr.get(), position);
    return (rc == CASS_OK);
}

auto Statement::BindNull(
    std::string_view name
) -> bool
{
    CassError rc = cass_statement_bind_null_by_name_n(m_cass_statement_ptr.get(), name.data(), name.length());
    return (rc == CASS_OK);
}

auto Statement::BindUuid(
    std::string_view uuid,
    size_t position
) -> bool
{
    CassUuid  cass_uuid;
    CassError rc = cass_uuid_from_string_n(uuid.data(), uuid.length(), &cass_uuid);
    if (rc != CASS_OK)
    {
        return false;
    }
    rc = cass_statement_bind_uuid(m_cass_statement_ptr.get(), position, cass_uuid);
    return (rc == CASS_OK);
}

auto Statement::BindText(
    std::string_view data,
    size_t position
) -> bool
{
    CassError rc = cass_statement_bind_string_n(m_cass_statement_ptr.get(), position, data.data(), data.length());
    return (rc == CASS_OK);
}

auto Statement::BindTinyInt(
    int8_t value,
    size_t position
) -> bool
{
    CassError rc = cass_statement_bind_int8(m_cass_statement_ptr.get(), position, value);
    return (rc == CASS_OK);
}

auto Statement::BindInt(int32_t value, size_t position) -> bool
{
    CassError rc = cass_statement_bind_int32(m_cass_statement_ptr.get(), position, value);
    return (rc == CASS_OK);
}

auto Statement::BindBigInt(int64_t value, size_t position) -> bool
{
    CassError rc = cass_statement_bind_int64(m_cass_statement_ptr.get(), position, value);
    return (rc == CASS_OK);
}

auto Statement::BindDouble(
    double value,
    size_t position
) -> bool
{
    CassError rc = cass_statement_bind_double(m_cass_statement_ptr.get(), position, value);
    return (rc == CASS_OK);
}

auto Statement::BindFloat(
    float value,
    size_t position
) -> bool
{
    CassError rc = cass_statement_bind_float(m_cass_statement_ptr.get(), position, value);
    return (rc == CASS_OK);
}

Statement::Statement(
    const CassPrepared* cass_prepared
)
    : m_cass_statement_ptr(cass_prepared_bind(cass_prepared))
{

}

} // namespace priam
