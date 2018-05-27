#pragma once

#include "priam/CppDriver.h"
#include "priam/Row.h"

#include <memory>
#include <vector>
#include <functional>

namespace priam
{

class Client;

/**
 * @param ce Convert this CassError into a string.
 * @return String representation of CassError value.
 */
auto to_string(CassError ce) -> const std::string&;

class Result
{
    /**
     * Client is a friend to call a Result's private constructor.
     */
    friend Client;
public:
    Result(const Result&) = delete;                     ///< No copying.
    Result(Result&&) = default;                         ///< Can move.
    auto operator=(const Result&) -> Result& = delete;
    auto operator=(Result&&) -> Result& = default;

    ~Result() = default;

    /**
     * @return Gets the status code of the query.
     * CassError::CASS_OK means the query completed successfully
     * CassError::CASS_ERROR_LIB_REQUEST_TIMED_OUT means the query timed out.
     * There are lots of other errors that can occur in the underlying cassandra driver.
     */
    auto GetStatusCode() const -> CassError;

    /**
     * @return Gets the number of rows returned by the query.
     */
    auto GetRowCount() const -> size_t;

    /**
     * @return Gets the number of columns in each row returned by the query.
     */
    auto GetColumnCount() const -> size_t;

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
    auto ForEachRow(
        Functor&& row_callback
    ) const -> void;

private:
    CassFuturePtr m_cass_future_ptr{nullptr};        ///< The underlying query future.
    CassResultPtr m_cass_result_ptr{nullptr};        ///< The underlying cassandra result object.
    CassError m_cass_error_code{CassError::CASS_OK}; ///< The query future error code.

    /**
     * @param query_future The underlying cassandra query future.  The result takes ownership and will
     *                     delete the query_future upon destruction.
     */
    explicit Result(
        CassFuture* query_future
    );
};

} // namespace priam

#include "priam/Result.tcc"
