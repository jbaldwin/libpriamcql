#pragma once

#include "vidar/Vidar.h"

namespace vidar
{

class Prepared;
class Client;

class Statement
{
    friend Prepared;
    friend Client;
public:
    ~Statement();

    auto BindUuid(const std::string& uuid, size_t position) -> bool;

    auto BindString(const std::string& data, size_t position) -> bool;

    auto BindInt32(uint32_t, size_t position) -> bool;

    auto BindInt64(uint64_t, size_t position) -> bool;

private:
    explicit Statement(const Prepared& prepared);

    CassStatement* m_statement;
};

} // namespace vidar
