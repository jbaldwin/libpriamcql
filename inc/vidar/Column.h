#pragma once

#include "vidar/CQL.h"

#include <ctime>

namespace vidar
{

/**
 * @param type Converts this CassValueType into a std::string representation.
 * @return The string representation.
 */
auto to_string(CassValueType type) -> const std::string&;

class Column
{
    /**
     * Allow Row's to create Column objects via the private constructor.
     */
    friend Row;
public:
    Column(const Column&) = delete;
    Column(Column&&) = delete;
    auto operator=(const Column&) -> Column& = delete;
    auto operator=(Column&&) -> Column&& = delete;

    /**
     * @return The data type of this column.
     */
    auto GetDataType() const -> CassValueType;

    /**
     * @return Turns the column value into a std::string.
     */
    auto GetASCII() const -> std::string;

    /**
     * @return Turns the column value into a timestamp.
     */
    auto GetTimestamp() const -> std::time_t;

    /**
     * @return Turns the column value into a date formatted timestamp.
     */
    auto GetTimestampAsDateFormatted() const -> std::string;

//    /**
//     * @return The column as a int32.
//     */
//    auto GetInt32() const -> int32_t;
//
//    /**
//     * @return The column as an uint32.
//     */
//    auto GetUInt32() const -> uint32_t;

private:
    const CassValue* m_cass_column{nullptr}; ///< The underlying cassandra value for this column/value.

    /**
     * Creates a column/value out of the underlying cassandra column/value.
     * @param cass_column Pointer to the cassandra driver value for this column.
     */
    explicit Column(
        const CassValue* cass_column
    );
};

} // namespace vidar

