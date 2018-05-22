#pragma once

#include "vidar/CQL.h"

namespace vidar
{

class Client;
class ResultIterator;

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
    Result(const Result&) = delete;
    Result(Result&&) noexcept ;
    auto operator=(const Result&) -> Result& = delete;
    auto operator=(Result&&) noexcept -> Result&;

    ~Result();

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
     * @return Gets an interator to traverse the rows returned by the query.
     */
    auto GetIterator() const -> ResultIterator;
private:
    CassFuture* m_cass_future{nullptr};              ///< The underlying query future.
    const CassResult* m_cass_result{nullptr};        ///< The underlying cassandra result object.
    CassError m_cass_error_code{CassError::CASS_OK}; ///< The query future error code.

    /**
     * @param query_future The underlying cassandra query future.
     */
    explicit Result(
        CassFuture* query_future
    );
};

} // namespace vidar
