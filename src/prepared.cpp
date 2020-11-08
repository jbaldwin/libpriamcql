#include "priam/prepared.hpp"
#include "priam/client.hpp"

#include <algorithm>
#include <stdexcept>

namespace priam
{
auto prepared::make_statement() const -> statement
{
    return statement{m_cass_prepared_ptr.get(), m_parameter_count};
}

prepared::prepared(client& client, std::string_view query)
    : m_parameter_count(std::count(query.begin(), query.end(), '?'))
{
    auto prepare_future =
        cass_future_ptr(cass_session_prepare_n(client.m_cass_session_ptr.get(), query.data(), query.length()));
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
