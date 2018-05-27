#include "priam/Row.h"

#include <stdexcept>
#include <functional>

namespace priam
{

auto Row::GetColumn(
    std::string_view name
) -> Value
{
    const CassValue* cass_column = cass_row_get_column_by_name_n(
        m_cass_row,
        name.data(),
        name.size()
    );

    if(cass_column != nullptr)
    {
        return Value(cass_column);
    }

    throw std::runtime_error("Row: Value name " + std::string(name) + " does not exist.");
}

auto Row::operator[](std::string_view name) -> Value
{
    return GetColumn(name);
}

auto Row::GetColumn(
    size_t column_idx
) -> Value
{
    const CassValue* cass_column = cass_row_get_column(m_cass_row, column_idx);

    if(cass_column != nullptr)
    {
        return Value(cass_column);
    }

    throw std::out_of_range("Row: Value idx " + std::to_string(column_idx) + " is out of bounds.");
}

auto Row::operator[](size_t column_idx) -> Value
{
    return GetColumn(column_idx);
}

auto Row::ForEachColumn(
    std::function<void(const Value&)> value_callback
) const -> void
{
    CassIteratorPtr cass_iterator_ptr(cass_iterator_from_row(m_cass_row));

    while(cass_iterator_next(cass_iterator_ptr.get()))
    {
        const CassValue* cass_value = cass_iterator_get_column(cass_iterator_ptr.get());
        priam::Value value(cass_value);
        value_callback(value);
    }
}

Row::Row(
    const CassRow* cass_row
)
    : m_cass_row(cass_row)
{

}

} // namespace priam
