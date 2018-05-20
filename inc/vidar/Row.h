#pragma once

#include "vidar/Vidar.h"

namespace vidar
{

class Column;

class Row
{
public:
    explicit Row(
        const CassRow* cass_row
    );
    Row(const Row&) = delete;
    Row(Row&&) = delete;
    auto operator=(const Row&) -> Row& = delete;
    auto operator=(Row&&) -> Row& = delete;

    auto GetColumn(std::string_view name) -> Column;
    auto GetColumn(size_t column_idx) -> Column;

private:
    const CassRow* m_cass_row{nullptr};
};

} // namespace vidar
