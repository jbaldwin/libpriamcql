#pragma once

#include <cstdint>

namespace priam
{
class duration
{
public:
    duration(int32_t months, int32_t days, int64_t nanos) : m_months(months), m_days(days), m_nanos(nanos) {}

    /**
     * @return Duration month amount.
     */
    auto months() const -> int32_t { return m_months; }

    /**
     * @return Duration day amount.
     */
    auto days() const -> int32_t { return m_days; }

    /**
     * @return Duration nanos amount.
     */
    auto nanos() const -> int64_t { return m_nanos; }

private:
    const int32_t m_months{0};
    const int32_t m_days{0};
    const int64_t m_nanos{0};
};

} // namespace priam
