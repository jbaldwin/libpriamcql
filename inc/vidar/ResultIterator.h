#pragma once

#include "vidar/CQL.h"

namespace vidar
{

class Row;
class Result;

class ResultIterator
{
    /**
     * Result creates a ResultIterator that can cycle through all the rows in the result, this needs
     * access to the private constructor.
     */
    friend Result;
public:
    ResultIterator(const ResultIterator&) = delete;
    ResultIterator(ResultIterator&&) = delete;
    auto operator=(const ResultIterator&) -> ResultIterator& = delete;
    auto operator=(ResultIterator&&) -> ResultIterator& = delete;

    ~ResultIterator();

    /**
     * @throws std::runtime_error If there are no more rows.  Use the Result::GetRowCount() to avoid going
     *                            out of bounds or catching an exception to determine when there are no more rows.
     * @return Gets the next row in the Result.
     */
    auto GetNextRow() -> Row;

private:
    CassIterator* m_cass_iterator{nullptr};

    /**
     * @param cass_iterator The underlying cass_iterator to go through this Result's rows.
     */
    explicit ResultIterator(
        CassIterator* cass_iterator
    );
};

} // namespace vidar
