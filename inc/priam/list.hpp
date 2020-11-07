#pragma once

#include "priam/blob.hpp"
#include "priam/cpp_driver.hpp"
#include "priam/decimal.hpp"
#include "priam/duration.hpp"
#include "priam/value.hpp"

#include <string_view>

namespace priam
{
class statement;

class statement_list
{
    friend statement;

public:
    /**
     * @param reserve_size The approximate number of items in the collection.
     */
    explicit statement_list(size_t reserve_size);
    statement_list(const statement_list&) = delete;
    statement_list(statement_list&& other) : m_cass_collection_ptr(std::move(other.m_cass_collection_ptr)) {}
    auto operator=(const statement_list&) -> statement_list& = delete;
    auto operator                                            =(statement_list&& other) -> statement_list&
    {
        if (std::addressof(other) != this)
        {
            m_cass_collection_ptr = std::move(other.m_cass_collection_ptr);
        }

        return *this;
    }

    ~statement_list() = default;

    //    auto append_custom() -> bool;
    auto append_ascii(std::string_view data) -> bool;
    auto append_big_int(int64_t value) -> bool;
    auto append_blob(blob blob) -> bool;
    auto append_boolean(bool value) -> bool;
    auto append_counter(int64_t value) -> bool;
    auto append_decimal(decimal value) -> bool;
    auto append_double(double value) -> bool;
    auto append_float(float value) -> bool;
    auto append_int(int32_t value) -> bool;
    auto append_text(std::string_view data) -> bool;
    auto append_timestamp(std::time_t timestamp) -> bool;
    auto append_uuid(std::string_view uuid) -> bool;
    auto append_varchar(std::string_view data) -> bool;
    auto append_time_uuid(std::string_view timeuuid) -> bool;
    auto append_inet(std::string_view inet) -> bool;
    auto append_date(uint32_t date) -> bool;
    auto append_time(int64_t time) -> bool;
    auto append_tiny_int(int8_t value) -> bool;
    auto append_duration(duration duration) -> bool;
    auto append_list(statement_list list) -> bool;
    //    auto append_map(StatementMap map) -> bool;
    //    auto append_tuple(StatementTuple tuple) -> bool;

private:
    cass_collection_ptr m_cass_collection_ptr{nullptr};
};

class result_list
{
    /// For private constructor.
    friend class value;

public:
    /**
     * Iterates over each Value in the List and calls the 'value_callback' for each Value.
     * @tparam functor_type The type signature for the functor 'value_callback'.
     * @param value_callback Functor Callback functor for each value in the list.  The parameter is 'const
     * priam::value&' and each callback returns void.
     */
    template<typename functor_type>
    auto for_each(functor_type&& value_callback) const -> void
    {
        cass_iterator_ptr cass_iterator_ptr{cass_iterator_from_collection(m_cass_value)};

        while (cass_iterator_next(cass_iterator_ptr.get()))
        {
            const CassValue* cass_value = cass_iterator_get_value(cass_iterator_ptr.get());
            if (cass_value != nullptr)
            {
                const priam::value value{cass_value};
                value_callback(value);
            }
        }
    }

private:
    /// Non-owning borrow of the result list.
    const CassValue* m_cass_value{nullptr};

    explicit result_list(const CassValue* cass_value);
};

} // namespace priam
