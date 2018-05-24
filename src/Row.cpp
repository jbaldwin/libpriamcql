#include "vidar/Row.h"

#include <stdexcept>
#include <functional>

namespace vidar
{

auto Row::GetColumn(
    std::string_view name
) -> Column
{
    const CassValue* cass_column = cass_row_get_column_by_name_n(
        m_cass_row,
        name.data(),
        name.size()
    );

    if(cass_column != nullptr)
    {
        return Column(cass_column);
    }

    throw std::runtime_error("Row: Column name " + std::string(name) + " does not exist.");
}

auto Row::GetColumn(
    size_t column_idx
) -> Column
{
    const CassValue* cass_column = cass_row_get_column(m_cass_row, column_idx);

    if(cass_column != nullptr)
    {
        return Column(cass_column);
    }

    throw std::out_of_range("Row: Column idx " + std::to_string(column_idx) + " is out of bounds.");
}

auto Row::ForEachColumn(
    std::function<void(const Column&)> column_callback
) const -> void
{
    CassIteratorPtr cass_iterator_ptr(cass_iterator_from_row(m_cass_row));

    while(cass_iterator_next(cass_iterator_ptr.get()))
    {
        const CassValue* cass_value = cass_iterator_get_column(cass_iterator_ptr.get());
        vidar::Column column(cass_value);
        column_callback(column);
    }
}

Row::Row(
    const CassRow* cass_row
)
    : m_cass_row(cass_row)
{

}

} // namespace vidar
