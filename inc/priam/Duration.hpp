#pragma once

#include <cstdint>

namespace priam {

class Duration {
public:
    Duration(
        int32_t months,
        int32_t days,
        int64_t nanos)
        : m_months(months)
        , m_days(days)
        , m_nanos(nanos)
    {
    }

    /**
     * @return Duration month amount.
     */
    auto Months() const -> int32_t { return m_months; }

    /**
     * @return Duration day amount.
     */
    auto Days() const -> int32_t { return m_days; }

    /**
     * @return Duration nanos amount.
     */
    auto Nanos() const -> int64_t { return m_nanos; }

private:
    int32_t m_months { 0 };
    int32_t m_days { 0 };
    int64_t m_nanos { 0 };
};

} // namespace priam
