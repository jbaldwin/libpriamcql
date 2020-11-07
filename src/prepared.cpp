#include "priam/prepared.hpp"
#include "priam/client.hpp"

#include <stdexcept>

namespace priam
{
auto prepared::create_statement() const -> std::unique_ptr<Statement>
{
    return std::unique_ptr<Statement>(new Statement(m_cass_prepared_ptr.get()));
}

prepared::prepared(client& client, const std::string& query)
{
    auto prepare_future =
        cass_future_ptr(cass_session_prepare_n(client.m_cass_session_ptr.get(), query.c_str(), query.length()));
    CassError rc = cass_future_error_code(prepare_future.get());

    if (rc == CASS_OK)
    {
        m_cass_prepared_ptr = cass_prepared_ptr(cass_future_get_prepared(prepare_future.get()));
    }
    else
    {
        throw std::runtime_error(std::string("cass_session_prepare() failed ").append(cass_error_desc(rc)));
    }
}

} // namespace priam
