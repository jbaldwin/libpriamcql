#pragma once

#include "priam/blob.hpp"

#include <cstdint>

namespace priam
{
class decimal
{
public:
    decimal(blob varint, int32_t scale) : m_varint(varint), m_scale(scale) {}

    /**
     * @return Gets the variable integer value for the Decimal.
     */
    auto varint() const -> const blob& { return m_varint; }

    /**
     * @return Gets the scale for the variable integer value.
     */
    auto scale() const -> int32_t { return m_scale; }

private:
    blob    m_varint{nullptr, 0};
    int32_t m_scale{0};
};

} // namespace priam
