#include "priam/result.hpp"
#include "priam/cpp_driver.hpp"

namespace priam
{
auto result::begin() const -> iterator
{
    if (m_cass_result_ptr == nullptr)
    {
        return end();
    }

    cass_iterator_ptr cass_iterator_ptr{cass_iterator_from_result(m_cass_result_ptr.get())};
    if (cass_iterator_ptr == nullptr)
    {
        return end();
    }

    if (!cass_iterator_next(cass_iterator_ptr.get()))
    {
        return end();
    }

    const CassRow* cass_row = cass_iterator_get_row(cass_iterator_ptr.get());
    if (cass_row == nullptr)
    {
        return end();
    }
    return iterator{std::move(cass_iterator_ptr), cass_row};
}

auto result::end() const -> iterator
{
    return iterator{nullptr, nullptr};
}

result::result(CassFuture* query_future)
    : m_cass_future_ptr(query_future),
      m_cass_result_ptr(cass_future_get_result(m_cass_future_ptr.get())),
      m_status(static_cast<priam::status>(cass_future_error_code(m_cass_future_ptr.get())))
{
}

} // namespace priam
