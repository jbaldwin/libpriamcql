#include "priam/Row.hpp"

#include <functional>
#include <stdexcept>

namespace priam {

auto Row::Column(
    std::string_view name) const -> Value
{
    const CassValue* cass_column = cass_row_get_column_by_name_n(
        m_cass_row,
        name.data(),
        name.size());

    if (cass_column != nullptr) {
        return Value(cass_column);
    }

    throw std::runtime_error("Row: Value name " + std::string(name) + " does not exist.");
}

auto Row::operator[](std::string_view name) const -> Value
{
    return Column(name);
}

auto Row::Column(
    size_t column_idx) const -> Value
{
    const CassValue* cass_column = cass_row_get_column(m_cass_row, column_idx);

    if (cass_column != nullptr) {
        return Value(cass_column);
    }

    throw std::out_of_range("Row: Value idx " + std::to_string(column_idx) + " is out of bounds.");
}

auto Row::operator[](size_t column_idx) const -> Value
{
    return Column(column_idx);
}

Row::Row(
    const CassRow* cass_row)
    : m_cass_row(cass_row)
{
}

} // namespace priam
