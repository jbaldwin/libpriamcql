#pragma once

#include "priam/blob.hpp"

#include <cstdint>

namespace priam
{
class decimal
{
public:
    decimal(blob variable_int, int32_t scale) : m_variable_int(variable_int), m_scale(scale) {}

    /**
     * @return Gets the variable integer value for the Decimal.
     */
    auto variable_int() const -> const blob& { return m_variable_int; }

    /**
     * @return Gets the scale for the variable integer value.
     */
    auto scale() const -> int32_t { return m_scale; }

private:
    blob    m_variable_int{nullptr, 0};
    int32_t m_scale{0};
};

} // namespace priam
