#include "vidar/Column.h"

namespace vidar
{

auto Column::GetDataType() const -> CassValueType
{
    const CassDataType* cass_data_type = cass_value_data_type(m_cass_column);
    if(cass_data_type != nullptr)
    {
        return cass_data_type_type(cass_data_type);
    }

    return CassValueType::CASS_VALUE_TYPE_UNKNOWN;
}

auto Column::GetInt32() const -> int32_t
{
    int32_t output = 0;
    cass_value_get_int32(m_cass_column, &output);
    return output;
}

auto Column::GetUInt32() const -> uint32_t
{
    uint32_t output = 0;
    cass_value_get_uint32(m_cass_column, &output);
    return output;
}

Column::Column(
    const CassValue* cass_column
)
    : m_cass_column(cass_column)
{

}

} // namespace vidar
