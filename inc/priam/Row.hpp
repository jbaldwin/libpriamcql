#pragma once

#include "priam/Value.hpp"
#include "priam/cpp_driver.hpp"

#include <functional>

namespace priam
{
class Result;

class Row
{
    friend Result; ///< For private constructor, only Result's can create Rows.
public:
    Row(const Row&) = delete;
    Row(Row&&)      = delete;
    auto operator=(const Row&) -> Row& = delete;
    auto operator=(Row &&) -> Row& = delete;

    /**
     * @param name The column's name to fetch.
     * @throws std::runtime_error If the column does not exist.
     * @return The column's Value.
     */
    auto Column(std::string_view name) const -> Value;

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
    auto Column(size_t column_idx) const -> Value;

    /**
     * @param column_idx The column's index to fetch.
     * @throws std::out_of_range If the column index requested is out of bounds.
     * @return The column's Value.
     */
    auto operator[](size_t column_idx) const -> Value;

    /**
     * Iterate over each column's Value in the Row.  The functor takes a single parameter `const priam::Value&`.
     * @param value_callback Callback function to be called on each column Value.
     */
    template<typename Functor>
    auto ForEachColumn(Functor&& value_callback) const -> void;

private:
    const CassRow* m_cass_row{
        nullptr}; ///< The underlying cassandra driver row object, this object does not need to be free'ed.

    /**
     * @param cass_row The underlying cassandra row object.
     */
    explicit Row(const CassRow* cass_row);
};

} // namespace priam

#include "priam/Row.tcc"
