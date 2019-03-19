#pragma once

#include "priam/Blob.h"

#include <cstdint>

namespace priam {

class Decimal {
public:
    Decimal(
        Blob variable_int,
        int32_t scale)
        : m_variable_int(variable_int)
        , m_scale(scale)
    {
    }

    /**
     * @return Gets the variable integer value for the Decimal.
     */
    auto GetVariableInt() const -> const Blob&
    {
        return m_variable_int;
    }

    /**
     * @return Gets the scale for the variable integer value.
     */
    auto GetScale() const -> int32_t
    {
        return m_scale;
    }

private:
    Blob m_variable_int { nullptr, 0 };
    int32_t m_scale { 0 };
};

} // namespace priam
