#include "vidar/Result.h"

namespace vidar
{

Result::Result(
    const CassResult* cass_result,
    CassError cass_error_code
)
    : m_cass_result(cass_result),
      m_cass_error_code(cass_error_code)
{

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
