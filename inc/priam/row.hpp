#pragma once

#include "priam/Value.hpp"
#include "priam/cpp_driver.hpp"

namespace priam
{
class result;

class row
{
    /// For private constructor, only result's can create rows.
    friend result;

public:
    row(const row&) = delete;
    row(row&&)      = delete;
    auto operator=(const row&) -> row& = delete;
    auto operator=(row &&) -> row& = delete;

    /**
     * @param name The column's name to fetch.
     * @throws std::runtime_error If the column does not exist.
     * @return The column's Value.
     */
    auto column(std::string_view name) const -> Value;

    /**
     * @param name The column's name to fetch.
     * @throws std::runtime_error If the column does not exist.
     * @return The column's Value.
     */
    auto operator[](std::string_view name) const -> Value;

    /**
     * @param column_idx The column's index to fetch.
     * @throws std::out_of_range If the column index requested is out of bounds.
     * @return The column's Value.
     */
    auto column(size_t column_idx) const -> Value;

    /**
     * @param column_idx The column's index to fetch.
     * @throws std::out_of_range If the column index requested is out of bounds.
     * @return The column's Value.
     */
    auto operator[](size_t column_idx) const -> Value;

    /**
     * Iterate over each column's Value in the row.  The functor takes a single parameter `const priam::Value&`.
     * @param value_callback Callback function to be called on each column Value.
     */
    template<typename functor_type>
    auto for_each(functor_type&& value_callback) const -> void
    {
        cass_iterator_ptr cass_iterator_ptr(cass_iterator_from_row(m_cass_row));

        while (cass_iterator_next(cass_iterator_ptr.get()))
        {
            const CassValue*   cass_value = cass_iterator_get_column(cass_iterator_ptr.get());
            const priam::Value value(cass_value);
            value_callback(value);
        }
    }

private:
    /// The underlying cassandra driver row object, this object does not need to be free'ed.
    const CassRow* m_cass_row{nullptr};

    /**
     * @param cass_row The underlying cassandra row object.
     */
    explicit row(const CassRow* cass_row);
};

} // namespace priam
