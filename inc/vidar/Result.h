#pragma once

#include "vidar/Vidar.h"

namespace vidar
{

class ResultIterator;

class Result
{
public:
    explicit Result(const CassResult* cass_result);
    Result(const Result&) = delete;
    Result(Result&&) = delete;
    auto operator=(const Result&) -> Result& = delete;
    auto operator=(Result&&) -> Result& = delete;

    auto GetRowCount() const -> size_t;
    auto GetColumnCount() const -> size_t;

    auto GetIterator() const -> ResultIterator;
private:
    const CassResult* m_cass_result;
};

} // namespace vidar
