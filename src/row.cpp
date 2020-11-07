#include "priam/row.hpp"

#include <functional>
#include <stdexcept>

namespace priam
{
auto row::column(std::string_view name) const -> value
{
    const CassValue* cass_column = cass_row_get_column_by_name_n(m_cass_row, name.data(), name.size());

    if (cass_column != nullptr)
    {
        return value{cass_column};
    }

    throw std::runtime_error("priam::row: value name " + std::string(name) + " does not exist.");
}

auto row::operator[](std::string_view name) const -> value
{
    return column(name);
}

auto row::column(size_t column_idx) const -> value
{
    const CassValue* cass_column = cass_row_get_column(m_cass_row, column_idx);

    if (cass_column != nullptr)
    {
        return value{cass_column};
    }

    throw std::out_of_range("priam::row: value idx " + std::to_string(column_idx) + " is out of bounds.");
}

auto row::operator[](size_t column_idx) const -> value
{
    return column(column_idx);
}

row::row(const CassRow* cass_row) : m_cass_row(cass_row)
{
}

} // namespace priam
