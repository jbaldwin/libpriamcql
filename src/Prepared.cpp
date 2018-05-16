#include "vidar/Prepared.h"

namespace vidar
{

auto Prepared::CreateStatement() const -> std::unique_ptr<Statement>
{
    return std::make_unique<Statement>(*this);
}

Prepared::Prepared(Client& client, const std::string& query)
{
    CassFuture* prepare_future = cass_session_prepare_n(client.m_session, query.c_str(), query.length());
    CassError rc = cass_future_error_code(prepare_future);

    if(rc == CASS_OK)
    {
        m_prepared = cass_future_get_prepared(prepare_future);
        cass_future_free(prepare_future);
    }
    else
    {
        if(prepare_future != nullptr)
        {
            cass_future_free(prepare_future);
        }
        throw std::runtime_error(std::string("cass_session_prepare() failed ").append(cass_error_desc(rc)));
    }
}

} // namespace vidar
