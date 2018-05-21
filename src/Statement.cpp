#include "vidar/Statement.h"

namespace vidar
{

Statement::~Statement()
{
    if(m_cass_statement != nullptr)
    {
        cass_statement_free(m_cass_statement);
        m_cass_statement = nullptr;
    }
}

auto Statement::BindUuid(
    const std::string& uuid,
    size_t position
) -> bool
{
    CassUuid  cass_uuid;
    CassError rc = cass_uuid_from_string_n(uuid.c_str(), uuid.length(), &cass_uuid);
    if (rc != CASS_OK)
    {
        return false;
    }
    rc = cass_statement_bind_uuid(m_cass_statement, position, cass_uuid);
    return (rc == CASS_OK);
}

auto Statement::BindString(
    const std::string& data,
    size_t position
) -> bool
{
    CassError rc = cass_statement_bind_string_n(m_cass_statement, position, data.c_str(), data.length());
    return (rc == CASS_OK);
}

auto Statement::BindInt32(
    int32_t value,
    size_t position
) -> bool
{
    CassError rc = cass_statement_bind_int32(m_cass_statement, position, value);
    return (rc == CASS_OK);
}

auto Statement::BindInt64(
    int64_t value,
    size_t position
) -> bool
{
    CassError rc = cass_statement_bind_int64(m_cass_statement, position, value);
    return (rc == CASS_OK);
}

Statement::Statement(
    const CassPrepared* cass_prepared
)
    : m_cass_statement(cass_prepared_bind(cass_prepared))
{

}

} // namespace vidar
