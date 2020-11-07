#pragma once

#include "priam/cpp_driver.hpp"
#include "priam/decimal.hpp"
#include "priam/duration.hpp"
#include "priam/blob.hpp"

#include <string_view>

namespace priam
{
class Statement;

class StatementList
{
    friend Statement;

public:
    /**
     * @param item_count The approximate number of items in the collection.
     */
    explicit StatementList(size_t item_count);

    //    auto AppendCustom() -> bool;
    auto AppendASCII(std::string_view data) -> bool;
    auto AppendBigInt(int64_t value) -> bool;
    auto AppendBlob(blob blob) -> bool;
    auto AppendBoolean(bool value) -> bool;
    auto AppendCounter(int64_t value) -> bool;
    auto AppendDecimal(decimal value) -> bool;
    auto AppendDouble(double value) -> bool;
    auto AppendFloat(float value) -> bool;
    auto AppendInt(int32_t value) -> bool;
    auto AppendText(std::string_view data) -> bool;
    auto AppendTimestamp(std::time_t timestamp) -> bool;
    auto AppendUUID(std::string_view uuid) -> bool;
    auto AppendVarChar(std::string_view data) -> bool;
    auto AppendTimeUUID(std::string_view timeuuid) -> bool;
    auto AppendINet(std::string_view inet) -> bool;
    auto AppendDate(uint32_t date) -> bool;
    auto AppendTime(int64_t time) -> bool;
    auto AppendTinyInt(int8_t value) -> bool;
    auto AppendDuration(duration duration) -> bool;
    auto AppendList(StatementList list) -> bool;
    //    auto AppendMap(StatementMap map) -> bool;
    //    auto AppendTuple(StatementTuple tuple) -> bool;

private:
    cass_collection_ptr m_cass_collection_ptr{nullptr};
};

class ResultList
{
    friend class Value; ///< For constructor
public:
    /**
     * Iterates over each Value in the List and calls the 'value_callback' for each Value.
     * @tparam Functor The type signature for the functor 'value_callback'.
     * @param value_callback Functor Callback functor for each value in the list.  The parameter is 'const
     * priam::Value&' and each callback returns void.
     */
    template<typename Functor>
    auto ForEachValue(Functor&& value_callback) const -> void;

private:
    const CassValue* m_cass_value{nullptr};

    explicit ResultList(const CassValue* cass_value);
};

} // namespace priam

#include "priam/List.tcc"
