#pragma once

#include "priam/Row.hpp"
#include "priam/cpp_driver.hpp"

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace priam
{
class client;

/**
 * @param ce Convert this CassError into a human readable string.
 * @return Human readable string representation of CassError value.
 */
auto to_string(CassError ce) -> std::string;

class Result
{
    /**
     * Client is a friend to call a Result's private constructor.
     */
    friend client;

public:
    Result(const Result&) = delete;
    Result(Result&&)      = default;
    auto operator=(const Result&) -> Result& = delete;
    auto operator=(Result &&) -> Result& = default;

    ~Result() = default;

    /**
     * @return Gets the status code of the query.
     * CassError::CASS_OK means the query completed successfully
     * CassError::CASS_ERROR_LIB_REQUEST_TIMED_OUT means the query timed out.
     * There are lots of other errors that can occur in the underlying cassandra driver.
     */
    auto StatusCode() const -> CassError { return m_cass_error_code; }

    /**
     * @return True if this result returned zero rows.
     */
    auto empty() const -> bool { return RowCount() == 0; }

    /**
     * @return Gets the number of rows returned by the query.
     */
    auto RowCount() const -> size_t { return cass_result_row_count(m_cass_result_ptr.get()); }

    /**
     * @return Gets the number of columns in each row returned by the query.
     */
    auto ColumnCount() const -> size_t { return cass_result_column_count(m_cass_result_ptr.get()); }

    /**
     * Iterators over each Row in the result.  The functor takes a single parameter `const priam::Row&`.
     *
     * Note that the underlying driver does not allow for anything but a forward iterator
     * and it invalidates previous rows if the iterator is moved forward.  This method of iteration
     * guarantees the client is only ever accessing a single Row at any given time per iteration.
     *
     * It is safe to iterate over the Row results concurrently.
     */
    template<typename Functor>
    auto ForEachRow(Functor&& row_callback) const -> void;

private:
    /// The underlying query future.
    cass_future_ptr m_cass_future_ptr{nullptr};
    /// The underlying cassandra result object.
    cass_result_ptr m_cass_result_ptr{nullptr};
    /// The query future error code.
    CassError m_cass_error_code{CassError::CASS_OK};

    /**
     * @param query_future The underlying cassandra query future.  The result takes ownership and will
     *                     delete the query_future upon destruction.
     */
    explicit Result(CassFuture* query_future);
};

} // namespace priam

#include "priam/Result.tcc"
