#pragma once

#include "priam/cpp_driver.hpp"
#include "priam/row.hpp"
#include "priam/status.hpp"

#include <chrono>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include <iostream>

namespace priam
{
class client;

class result
{
    /// Client is a friend to call a result's private constructor.
    friend client;

public:
    class iterator
    {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type        = priam::row;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const priam::row*;
        using reference         = const priam::row&;

        iterator(cass_iterator_ptr iter_ptr, const CassRow* cass_row)
            : m_iter_ptr(std::move(iter_ptr)),
              m_cass_row(cass_row)
        {
        }
        iterator(const iterator&) = delete;
        iterator(iterator&& other)
            : m_iter_ptr(std::move(other.m_iter_ptr)),
              m_cass_row(std::exchange(other.m_cass_row, nullptr))
        {
        }

        auto operator=(const iterator&) noexcept -> iterator& = delete;
        auto operator                                         =(iterator&& other) noexcept -> iterator&
        {
            if (std::addressof(other) != this)
            {
                m_iter_ptr = std::move(other.m_iter_ptr);
                m_cass_row = std::exchange(other.m_cass_row, nullptr);
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
            return iterator{std::move(m_iter_ptr), std::exchange(m_cass_row, nullptr)};
        }

        auto operator*() -> priam::row { return priam::row{m_cass_row}; }

        auto operator==(const iterator& other) const -> bool { return m_cass_row == other.m_cass_row; }

        auto operator!=(const iterator& other) const -> bool { return !(*this == other); }

    private:
        /// The iterator must maintain the lifetime of the cassandra driver's iterator.
        cass_iterator_ptr m_iter_ptr{nullptr};
        /// The current row that the iterator is on.
        const CassRow* m_cass_row{nullptr};

        auto advance() -> void
        {
            if (cass_iterator_next(m_iter_ptr.get()))
            {
                m_cass_row = cass_iterator_get_row(m_iter_ptr.get());
            }
            else
            {
                end();
            }
        }

        auto end() -> void
        {
            m_iter_ptr = nullptr;
            m_cass_row = nullptr;
        }
    };

    auto begin() const -> iterator;
    auto end() const -> iterator;

    result(const result&) = delete;
    result(result&&)      = default;
    auto operator=(const result&) -> result& = delete;
    auto operator=(result &&) -> result& = default;

    ~result() = default;

    /**
     * @return Gets the status code of the query.
     * status::ok means the query completed successfully
     * status::client_request_timed_out means the query timed out.
     * There are lots of other errors that can occur in the underlying cassandra driver.
     */
    auto status() const -> priam::status { return m_status; }

    /**
     * @return True if this result returned zero rows.
     */
    auto empty() const -> bool { return row_count() == 0; }

    /**
     * @return Gets the number of rows returned by the query.
     */
    auto size() const -> size_t { return row_count(); }

    /**
     * @return Gets the number of rows returned by the query.
     */
    auto row_count() const -> size_t { return cass_result_row_count(m_cass_result_ptr.get()); }

    /**
     * This is convience method for when selecting a row out of the db by its primary key and the
     * expectation is that there will only ever be 0 or 1 rows returned if it exists.
     * @return The first row in the result, be sure to check size >= 1 before calling.
     */
    auto first_row() const -> priam::row { return priam::row{cass_result_first_row(m_cass_result_ptr.get())}; }

    /**
     * @return Gets the number of columns in each row returned by the query.
     */
    auto column_count() const -> size_t { return cass_result_column_count(m_cass_result_ptr.get()); }

    /**
     * Iterators over each row in the result.  The functor takes a single parameter `const priam::row&`.
     *
     * Note that the underlying driver does not allow for anything but a input iterator
     * and it invalidates previous rows if the iterator is moved forward.  Do not access any
     * priam::row's after advancing past that row.
     *
     * It is safe to iterate over the row results concurrently.
     */
    template<typename functor_type>
    auto for_each(functor_type&& row_callback) const -> void
    {
        cass_iterator_ptr cass_iterator_ptr{cass_iterator_from_result(m_cass_result_ptr.get())};

        while (cass_iterator_next(cass_iterator_ptr.get()))
        {
            const CassRow* cass_row = cass_iterator_get_row(cass_iterator_ptr.get());
            if (cass_row != nullptr)
            {
                const priam::row row{cass_row};
                row_callback(row);
            }
        }
    }

private:
    /// The underlying query future.
    cass_future_ptr m_cass_future_ptr{nullptr};
    /// The underlying cassandra result object.
    cass_result_ptr m_cass_result_ptr{nullptr};
    /// The query future status.
    priam::status m_status{status::ok};

    /**
     * @param query_future The underlying cassandra query future.  The result takes ownership and will
     *                     delete the query_future upon destruction.
     */
    explicit result(CassFuture* query_future);
};

} // namespace priam
