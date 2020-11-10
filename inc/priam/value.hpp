#pragma once

#include "priam/blob.hpp"
#include "priam/cpp_driver.hpp"
#include "priam/decimal.hpp"
#include "priam/duration.hpp"
#include "priam/type.hpp"

#include <cstddef>
#include <ctime>
#include <optional>
#include <string>

namespace priam
{
class row;
class result_list;
class map;
class set;
class tuple;

/**
 * Encapsulates a column's Value returned from a query.
 *
 * Note the following data type conversions from C* to C++ here:
 * https://datastax.github.io/cpp-driver/topics/basics/
 */
class value
{
    friend row;         ///< For private constructor, rows can create values.
    friend result_list; ///< For private constructor, lists can create values.
    friend map;         ///< For private constructor, maps can create values.
    friend set;         ///< For private constructor, sets can create values.
    friend tuple;       ///< For private constructor, tuples can create values.
public:
    value(const value&) = delete;
    value(value&&)      = delete;
    auto operator=(const value&) -> value& = delete;
    auto operator=(value &&) -> value& = delete;

    /**
     * @return True if the value is null.
     */
    auto is_null() const -> bool;

    /**
     * @return The data type of this Value.
     */
    auto type() const -> data_type;

    /**
     * @tparam d to see if this value is that type.
     * @return True if tis value is the given data type.
     */
    template<data_type d>
    auto is() const -> bool
    {
        return type() == d;
    }

    // TODO: implement
    // auto as_custom() const -> void*;

    /**
     * @return Cassandra column data type 'ascii' into std::string.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_ascii() const -> std::optional<std::string>;

    /**
     * @return Cassandra column data type 'bigint' into int64_t.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_big_int() const -> std::optional<int64_t>;

    /**
     * @return Cassandra column data type 'blob' into Blob.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_blob() const -> std::optional<blob>;

    /**
     * @return Cassandra column data type 'boolean' into bool.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_boolean() const -> std::optional<bool>;

    /**
     * @return Cassandra column data type 'counter' into int64_t.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_counter() const -> std::optional<int64_t>;

    /**
     * @return Cassandra column data type 'decimal' into Decimal.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_decimal() const -> std::optional<decimal>;

    /**
     * @return Cassandra column data type 'double' into double.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_double() const -> std::optional<double>;

    /**
     * @return Cassandra column data type 'float' into float.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_float() const -> std::optional<float>;

    /**
     * @return Cassandra column data type 'int' into int32_t.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_int() const -> std::optional<int32_t>;

    /**
     * @return Cassandra column data type 'text' into std::string.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_text() const -> std::optional<std::string>;

    /**
     * @return Cassandra column data type 'timestamp' into std::time_t.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_timestamp() const -> std::optional<std::time_t>;

    /**
     * @return Cassandra column data type 'timestamp' into a date formatted timestamp.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_timestamp_date_formatted() const -> std::optional<std::string>;

    /**
     * @return Cassandra column data type 'uuid' into std::string uuid.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_uuid() const -> std::optional<uuid>;

    /**
     * @return Cassandra column data type 'varchar' into std::string.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_varchar() const -> std::optional<std::string>;

    /**
     * @return Cassandra column data type 'varint' into blob.  The blob contains the variable integer raw information.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_varint() const -> std::optional<blob>;

    /**
     * @return Cassandra column data type 'time uuid' into std::string.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_time_uuid() const -> std::optional<uuid>;

    /**
     * @return Cassandra column data type 'inet' into std::string.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_inet() const -> std::optional<std::string>;

    /**
     * @see http://datastax.github.io/cpp-driver/topics/basics/date_and_time/
     * @return Cassandra data type 'date' into uint32_t.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_date() const -> std::optional<uint32_t>;

    /**
     * @see http://datastax.github.io/cpp-driver/topics/basics/date_and_time/
     * @return Cassandra data type 'time' into int64_t.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_time() const -> std::optional<int64_t>;

    /**
     * @return Cassandra data type 'smallint' into int16_t.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_small_int() const -> std::optional<int16_t>;

    /**
     * @return Cassandra data type 'tinyint' into int8_t.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_tiny_int() const -> std::optional<int8_t>;

    /**
     * @return Cassandra data type 'duration' into Duration.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_duration() const -> std::optional<duration>;

    /**
     * @return Cassandra data type 'list' into priam::List.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_list() const -> std::optional<priam::result_list>;

    /**
     * @return Cassandra data type 'map' into priam::map.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_map() const -> std::optional<priam::map>;

    /**
     * @return Cassandra data type 'set' into priam::set.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_set() const -> std::optional<priam::set>;

    // TODO: implement
    // auto as_udt() const -> priam::udt;

    /**
     * @return Cassandra data type 'tuple' into priam::Tuple.
     *         If the value is null then std::nullopt is returned.
     */
    auto as_tuple() const -> std::optional<priam::tuple>;

private:
    /// The underlying cassandra value for this column/value, this object does not need to be free'ed.
    const CassValue* m_cass_value{nullptr};

    /**
     * Creates a column/value out of the underlying cassandra column/value.
     * @param cass_column Pointer to the cassandra driver value for this column.
     */
    explicit value(const CassValue* cass_column);
};

} // namespace priam
