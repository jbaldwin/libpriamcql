#pragma once

#include "vidar/Vidar.h"

namespace vidar
{

class Row;
class Result;

class ResultIterator
{
    friend Result;
public:
    ResultIterator(const ResultIterator&) = delete;
    ResultIterator(ResultIterator&&) = delete;
    auto operator=(const ResultIterator&) -> ResultIterator& = delete;
    auto operator=(ResultIterator&&) -> ResultIterator& = delete;

    ~ResultIterator();

    auto GetNextRow() -> Row;

private:
    CassIterator* m_cass_iterator{nullptr};

    explicit ResultIterator(
        CassIterator* cass_iterator
    );
};

} // namespace vidar
