#pragma once

#include "vidar/CQL.h"

namespace vidar
{

class Column;

class Row
{
    /**
     * ResultIterators create the rows out of the CassRow objects and need access to the private constructor.
     */
    friend ResultIterator;
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
    auto GetColumn(std::string_view name) -> Column;

    /**
     * @param column_idx The column's index to fetch.
     * @throws std::out_of_range If the column index requested is out of bounds.
     * @return The Column object.
     */
    auto GetColumn(size_t column_idx) -> Column;

private:
    const CassRow* m_cass_row{nullptr}; ///< The underlying cassandra driver row object.

    /**
     * @param cass_row The underlying cassandra row object.
     */
    explicit Row(
        const CassRow* cass_row
    );
};

} // namespace vidar
