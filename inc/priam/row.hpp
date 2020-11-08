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
    class iterator
    {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type        = priam::value;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const priam::value*;
        using reference         = const priam::value&;

        iterator(cass_iterator_ptr iter_ptr, const CassValue* cass_value)
            : m_iter_ptr(std::move(iter_ptr)),
              m_cass_value(cass_value)
        {
        }
        iterator(const iterator&) = delete;
        iterator(iterator&& other)
            : m_iter_ptr(std::move(other.m_iter_ptr)),
              m_cass_value(std::exchange(other.m_cass_value, nullptr))
        {
        }

        auto operator=(const iterator&) noexcept -> iterator& = delete;
        auto operator                                         =(iterator&& other) noexcept -> iterator&
        {
            if (std::addressof(other) != this)
            {
                m_iter_ptr   = std::move(other.m_iter_ptr);
                m_cass_value = std::exchange(other.m_cass_value, nullptr);
            }
            return *this;
        }

        auto operator++() -> iterator&
        {
            advance();
            return *this;
        }

        auto operator++(int) -> iterator
        {
            advance();
            return iterator{std::move(m_iter_ptr), std::exchange(m_cass_value, nullptr)};
        }

        auto operator*() -> priam::value { return priam::value{m_cass_value}; }

        auto operator==(const iterator& other) const -> bool { return m_cass_value == other.m_cass_value; }

        auto operator!=(const iterator& other) const -> bool { return !(*this == other); }

    private:
        /// The iterator must maintain the lifetime of the cassandra driver's iterator.
        cass_iterator_ptr m_iter_ptr{nullptr};
        /// The current value in the row.
        const CassValue* m_cass_value{nullptr};

        auto advance() -> void
        {
            if (cass_iterator_next(m_iter_ptr.get()))
            {
                m_cass_value = cass_iterator_get_column(m_iter_ptr.get());
            }
            else
            {
                end();
            }
        }

        auto end() -> void
        {
            m_iter_ptr   = nullptr;
            m_cass_value = nullptr;
        }
    };

    auto begin() const -> iterator;
    auto end() const -> iterator;

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
        if (m_cass_row != nullptr)
        {
            cass_iterator_ptr cass_iterator_ptr(cass_iterator_from_row(m_cass_row));

            while (cass_iterator_next(cass_iterator_ptr.get()))
            {
                const CassValue* cass_value = cass_iterator_get_column(cass_iterator_ptr.get());
                if (cass_value != nullptr)
                {
                    const priam::value value(cass_value);
                    value_callback(value);
                }
            }
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
