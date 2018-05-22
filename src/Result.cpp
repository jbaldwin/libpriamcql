#include "vidar/Result.h"

namespace vidar
{

Result::Result(
    CassFuture* query_future
)
    : m_cass_result(cass_future_get_result(query_future)),
      m_cass_error_code(cass_future_error_code(query_future))
{

}

Result::Result(Result&& other) noexcept
{
    m_cass_future = other.m_cass_future;
    m_cass_result = other.m_cass_result;
    m_cass_error_code = other.m_cass_error_code;

    other.m_cass_future = nullptr;
    other.m_cass_result = nullptr;
    other.m_cass_error_code = CassError::CASS_OK;
}

auto Result::operator=(Result&& other) noexcept -> Result&
{
    m_cass_future = other.m_cass_future;
    m_cass_result = other.m_cass_result;
    m_cass_error_code = other.m_cass_error_code;

    other.m_cass_future = nullptr;
    other.m_cass_result = nullptr;
    other.m_cass_error_code = CassError::CASS_OK;

    return *this;
}

Result::~Result()
{
    if(m_cass_future != nullptr)
    {
        cass_future_free(m_cass_future);
    }

    if(m_cass_result != nullptr)
    {
        cass_result_free(m_cass_result);
    }
}

auto Result::GetStatusCode() const -> CassError
{
    return m_cass_error_code;
}

auto Result::GetRowCount() const -> size_t
{
    return cass_result_row_count(m_cass_result);
}

auto Result::GetColumnCount() const -> size_t
{
    return cass_result_column_count(m_cass_result);
}

auto Result::GetIterator() const -> ResultIterator
{
    return ResultIterator(cass_iterator_from_result(m_cass_result));
}

} // namespace vidar
