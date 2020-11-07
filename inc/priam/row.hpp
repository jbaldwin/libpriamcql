#pragma once

#include "priam/cpp_driver.hpp"
#include "priam/value.hpp"

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
     * @return The column's value.
     */
    auto column(std::string_view name) const -> value;

    /**
     * @param name The column's name to fetch.
     * @throws std::runtime_error If the column does not exist.
     * @return The column's value.
     */
    auto operator[](std::string_view name) const -> value;

    /**
     * @param column_idx The column's index to fetch.
     * @throws std::out_of_range If the column index requested is out of bounds.
     * @return The column's value.
     */
    auto column(size_t column_idx) const -> value;

    /**
     * @param column_idx The column's index to fetch.
     * @throws std::out_of_range If the column index requested is out of bounds.
     * @return The column's value.
     */
    auto operator[](size_t column_idx) const -> value;

    /**
     * Iterate over each column's value in the row.  The functor takes a single parameter `const priam::value&`.
     * @param value_callback Callback function to be called on each column value.
     */
    template<typename functor_type>
    auto for_each(functor_type&& value_callback) const -> void
    {
        cass_iterator_ptr cass_iterator_ptr(cass_iterator_from_row(m_cass_row));

        while (cass_iterator_next(cass_iterator_ptr.get()))
        {
            const CassValue*   cass_value = cass_iterator_get_column(cass_iterator_ptr.get());
            const priam::value value(cass_value);
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
