#pragma once

#include "vidar/CQL.h"

namespace vidar
{

class Client;
class ResultIterator;

class Result
{
    /**
     * Client is a friend to call a Result's private constructor.
     */
    friend Client;
public:
    Result(const Result&) = delete;
    Result(Result&&) = delete;
    auto operator=(const Result&) -> Result& = delete;
    auto operator=(Result&&) -> Result& = delete;

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
    const CassResult* m_cass_result;    ///< The underlying cassandra result object.
    const CassError m_cass_error_code;  ///< The query future error code.

    /**
     * @param cass_result Creates a Result object around the underlying cassandra result object.
     * @param cass_error_code The future query status error code.
     */
    explicit Result(
        const CassResult* cass_result,
        CassError cass_error_code
    );
};

} // namespace vidar
