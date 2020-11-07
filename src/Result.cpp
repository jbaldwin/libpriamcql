#include "priam/Result.hpp"
#include "priam/cpp_driver.hpp"

namespace priam
{
auto to_string(CassError ce) -> std::string
{
    return cass_error_desc(ce);
}

Result::Result(CassFuture* query_future)
    : m_cass_future_ptr(query_future),
      m_cass_result_ptr(cass_future_get_result(m_cass_future_ptr.get())),
      m_cass_error_code(cass_future_error_code(m_cass_future_ptr.get()))
{
}

} // namespace priam
