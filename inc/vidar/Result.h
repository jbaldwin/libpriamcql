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

    auto GetRowCount() const -> size_t;
    auto GetColumnCount() const -> size_t;

    auto GetIterator() const -> ResultIterator;
private:
    const CassResult* m_cass_result;

    explicit Result(const CassResult* cass_result);
};

} // namespace vidar
