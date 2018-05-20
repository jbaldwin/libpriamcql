#include "vidar/Row.h"

namespace vidar
{

Row::Row(
    const CassRow* cass_row
)
    : m_cass_row(cass_row)
{

}

auto Row::GetColumn(std::string_view name) -> Column
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

auto Row::GetColumn(size_t column_idx) -> Column
{
    const CassValue* cass_column = cass_row_get_column(m_cass_row, column_idx);

    if(cass_column != nullptr)
    {
        return Column(cass_column);
    }

    throw std::out_of_range("Row: Column idx " + std::to_string(column_idx) + " is out of bounds.");
}

} // namespace vidar
