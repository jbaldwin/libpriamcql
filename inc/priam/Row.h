#pragma once

#include "priam/CppDriver.h"
#include "priam/Column.h"

#include <functional>

namespace priam
{

class Result;

class Row
{
    friend Result; ///< For private constructor, only Result's can create Rows.
public:
    Row(const Row&) = delete;
    Row(Row&&) = delete;
    auto operator=(const Row&) -> Row& = delete;
    auto operator=(Row&&) -> Row& = delete;

    /**
     * @param name The column's name to fetch.
     * @throws std::runtime_error If the column does not exist.
     * @return The Column object.
     */
    auto GetColumn(
        std::string_view name
    ) -> Column;

    /**
     * @param column_idx The column's index to fetch.
     * @throws std::out_of_range If the column index requested is out of bounds.
     * @return The Column object.
     */
    auto GetColumn(
        size_t column_idx
    ) -> Column;

    /**
     * Iterator over each Column in the Row.
     * @param column_callback Callback function to be called on each column value.
     */
    auto ForEachColumn(
        std::function<void(const priam::Column& column)> column_callback
    ) const -> void;

private:
    const CassRow* m_cass_row{nullptr}; ///< The underlying cassandra driver row object, this object does not need to be free'ed.

    /**
     * @param cass_row The underlying cassandra row object.
     */
    explicit Row(
        const CassRow* cass_row
    );
};

} // namespace priam
