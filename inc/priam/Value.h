#pragma once

#include "priam/CppDriver.h"

#include <ctime>
#include <string>
#include <variant>

namespace priam
{

class Row;

/**
 * @param type Converts this CassValueType into a std::string representation.
 * @return The string representation.
 */
auto to_string(CassValueType type) -> const std::string&;

/**
 * Encapsulates a column's Value returned from a query.
 *
 * Note the following data type conversions from C* to C++ here:
 * https://datastax.github.io/cpp-driver/topics/basics/
 */
class Value
{
    friend Row; ///< For private constructor, only Row's can create column Values.
public:
    Value(const Value&) = delete;
    Value(Value&&) = delete;
    auto operator=(const Value&) -> Value& = delete;
    auto operator=(Value&&) -> Value&& = delete;

    /**
     * Unfortunately the CassValueType does not support 'null'.
     * @return True if the value is null.
     */
    auto IsNull() const -> bool;

    /**
     * @return The data type of this Value.
     */
    auto GetDataType() const -> CassValueType;

    //auto GetCustom() const -> void*;

    /**
     * @return Turns the column value into a std::string.
     */
    auto GetASCII() const -> std::string;

    auto GetBigInt() const -> int64_t;

    //auto GetBlob() const -> std::vector<uint8_t>;

    auto GetBoolean() const -> bool;

    auto GetCounter() const -> int64_t;

    //auto GetDecimal() const -> std::variant<std::vector<uint8_t>, int32_t>;

    auto GetDouble() const -> double;

    auto GetFloat() const -> float;

    auto GetInt() const -> int32_t;

    auto GetText() const -> std::string;

    /**
     * @return Turns the column value into a timestamp.
     */
    auto GetTimestamp() const -> std::time_t;

    /**
     * @return Turns the column value into a date formatted timestamp.
     */
    auto GetTimestampAsDateFormatted() const -> std::string;

    auto GetUUID() const -> std::string;

    auto GetVarChar() const -> std::string;

    //auto GetVarInt() const -> std::vector<uint8_t>;

    auto GetTimeUUID() const -> std::string;

    //auto GetINet() const -> std::string;

    auto GetDate() const -> uint32_t;

    auto GetTime() const -> int64_t;

    auto GetSmallInt() const -> int16_t;

    auto GetTinyInt() const -> int8_t;

    // months/days/nanos TODO: create wrapper struct
    //auto GetDuration() const -> std::tuple<int32_t, int32_t, int64_t>;

    //auto GetList() const -> priam::List;

    //auto GetMap() const -> priam::Map;

    //auto GetSet() const -> priam::Set;

    //auto GetUDT() const -> priam::UDT;

    //auto GetTuple() const -> priam::Tuple;

private:
    const CassValue* m_cass_value{nullptr}; ///< The underlying cassandra value for this column/value, this object does not need to be free'ed.

    /**
     * Creates a column/value out of the underlying cassandra column/value.
     * @param cass_column Pointer to the cassandra driver value for this column.
     */
    explicit Value(
        const CassValue* cass_column
    );
};

} // namespace priam

