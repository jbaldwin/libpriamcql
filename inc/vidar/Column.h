#pragma once

#include "vidar/Vidar.h"

namespace vidar
{

class Column
{
public:
    explicit Column(
        const CassValue* cass_column
    );
    Column(const Column&) = delete;
    Column(Column&&) = delete;
    auto operator=(const Column&) -> Column& = delete;
    auto operator=(Column&&) -> Column&& = delete;

    auto GetDataType() const -> CassValueType;

    auto GetInt32() const -> int32_t;

    auto GetUInt32() const -> uint32_t;

private:
    const CassValue* m_cass_column{nullptr};
};

} // namespace vidar

