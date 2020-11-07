#pragma once

#include "priam/row.hpp"
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

class result
{
    /**
     * Client is a friend to call a result's private constructor.
     */
    friend client;

public:
    result(const result&) = delete;
    result(result&&)      = default;
    auto operator=(const result&) -> result& = delete;
    auto operator=(result &&) -> result& = default;

    ~result() = default;

    /**
     * @return Gets the status code of the query.
     * CassError::CASS_OK means the query completed successfully
     * CassError::CASS_ERROR_LIB_REQUEST_TIMED_OUT means the query timed out.
     * There are lots of other errors that can occur in the underlying cassandra driver.
     */
    auto status_code() const -> CassError { return m_cass_error_code; }

    /**
     * @return True if this result returned zero rows.
     */
    auto empty() const -> bool { return row_count() == 0; }

    /**
     * @return Gets the number of rows returned by the query.
     */
    auto size() const -> size_t { return row_count(); }

    /**
     * @return Gets the number of rows returned by the query.
     */
    auto row_count() const -> size_t { return cass_result_row_count(m_cass_result_ptr.get()); }

    /**
     * @return Gets the number of columns in each row returned by the query.
     */
    auto column_count() const -> size_t { return cass_result_column_count(m_cass_result_ptr.get()); }

    /**
     * Iterators over each row in the result.  The functor takes a single parameter `const priam::row&`.
     *
     * Note that the underlying driver does not allow for anything but a forward iterator
     * and it invalidates previous rows if the iterator is moved forward.  This method of iteration
     * guarantees the client is only ever accessing a single row at any given time per iteration.
     *
     * It is safe to iterate over the row results concurrently.
     */
    template<typename functor_type>
    auto for_each(functor_type&& row_callback) const -> void
    {
        cass_iterator_ptr cass_iterator_ptr{cass_iterator_from_result(m_cass_result_ptr.get())};

        while (cass_iterator_next(cass_iterator_ptr.get()))
        {
            const CassRow* cass_row = cass_iterator_get_row(cass_iterator_ptr.get());
            if (cass_row != nullptr)
            {
                const priam::row row{cass_row};
                row_callback(row);
            }
        }
    }

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
    explicit result(CassFuture* query_future);
};

} // namespace priam
