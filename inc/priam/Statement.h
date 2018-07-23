#pragma once

#include "priam/CppDriver.h"

#include <string>
#include <string_view>
#include <vector>

namespace priam
{

class Prepared;
class Client;

class Statement
{
    /**
     * Prepared objects are the only type allowed to create Statement's, it needs access to the
     * private constructor.
     */
    friend Prepared;

    /**
     * Client uses the underlying cassandra statement object when ExecuteQuery() is called.
     */
    friend Client;
public:
    Statement(const Statement&) = delete;
    Statement(Statement&&) = delete;
    auto operator=(const Statement&) -> Statement& = delete;
    auto operator=(Statement&&) -> Statement& = delete;

    ~Statement() = default;

    /**
     * @param position The bind position.
     * @return True if 'null' was correctly bound.
     */
    auto BindNull(
        size_t position
    ) -> bool;

    /**
     * @param name Parameter name to bind 'null' to.
     * @return True  if 'null' was correctly bound.
     */
    auto BindNull(
        std::string_view name
    ) -> bool;

    auto BindBoolean(
        bool value,
        size_t position
    ) -> bool;

    auto BindBoolean(
        bool value,
        std::string_view name
    ) -> bool;

    /**
     * @param uuid Bind this UUID to the prepared statement.
     * @param position The bind position.
     * @return True if the UUID was correctly bound.
     */
    auto BindUuid(
        std::string_view uuid,
        size_t position
    ) -> bool;

    /**
     * @param uuid Bind this UUID to the prepared statement.
     * @param name Parameter name to bind the UUID to.
     * @return True if the UUID was correctly bound.
     */
    auto BindUuid(
        std::string_view uuid,
        std::string_view name
    ) -> bool;

    /**
     * @param data Text data to bind to the prepared statement.
     * @param position The bind position.
     * @return True if the string was correctly bound.
     */
    auto BindText(
        std::string_view data,
        size_t position
    ) -> bool;

    /**
     * @param data Text data to bind to the prepared statement.
     * @param name Parameter name to bind the text to.
     * @return True if the text was correctly bound.
     */
    auto BindText(
        std::string_view data,
        std::string_view name
    ) -> bool;

    /**
     * @param value Bind this int 8 to the prepared statement.
     * @param position The bind position.
     * @return True if the int 8 was correctly bound.
     */
    auto BindTinyInt(
        int8_t value,
        size_t position
    ) -> bool;

    /**
     * @param value Bind this int 8 to the prepared statement.
     * @param name Parameter name to bind the int 8 to.
     * @return True if the int 8 was correctly bound.
     */
    auto BindTinyInt(
        int8_t value,
        std::string_view name
    ) -> bool;

    /**
     * @param value Bind this int 32 to the prepared statement.
     * @param position The bind position.
     * @return True if the int 32 was correctly bound.
     */
    auto BindInt(
        int32_t value,
        size_t position
    ) -> bool;

    /**
     * @param value Bind this int 32 to the prepared statement.
     * @param name Parameter name to bind the int 32 to.
     * @return True if the int 32 was correctly bound.
     */
    auto BindInt(
        int32_t value,
        std::string_view name
    ) -> bool;

    /**
     * @param value Bind this int 64 to the prepared statement.
     * @param position The bind position.
     * @return True if the int 64 was correctly bound.
     */
    auto BindBigInt(
        int64_t value,
        size_t position
    ) -> bool;

    /**
     * @param value Bind this int 64 to the prepared statement.
     * @param name Parameter name to bind the int 64 to.
     * @return True if the int 64 was correctly bound.
     */
    auto BindBigInt(
        int64_t value,
        std::string_view name
    ) -> bool;

    /**
     * @param value Bind this float to the prepared statement.
     * @param position The bind position.
     * @return True if the float was correctly bound.
     */
    auto BindFloat(
        float value,
        size_t position
    ) -> bool;

    /**
     * @param value Bind this float to the prepared statement.
     * @param name Parameter name to bind the float to.
     * @return True if the float was correctly bound.
     */
    auto BindFloat(
        float value,
        std::string_view name
    ) -> bool;

    /**
     * @param value Bind this double to the prepared statement.
     * @param position The bind position.
     * @return True if the double was correctly bound.
     */
    auto BindDouble(
        double value,
        size_t position
    ) -> bool;

    /**
     * @param value Bind this double to the prepared statement.
     * @param name Parameter name to bind the double to.
     * @return True if the double was correctly bound.
     */
    auto BindDouble(
        double value,
        std::string_view name
    ) -> bool;

    /**
     * @param values Bind these tiny ints to the prepared statement.
     * @param position The bind position.
     * @return True if the tiny ints were correctly bound.
     */
    template<typename T>
    auto BindListTinyInt(
        const std::vector<T>& values,
        size_t position
    ) -> bool;

    /**
     * @param values Bind these tiny ints to the prepared statement.
     * @param name Parameter name to bind the tiny ints to.
     * @return True if the tiny ints were correctly bound.
     */
    template<typename T>
    auto BindListTinyInt(
        const std::vector<T>& values,
        std::string_view name
    ) -> bool;
private:
    /**
     * Creates a Prepared Statement object from the provided underlying cassandra prepared object.
     * @param cass_prepared The underlying cassandra prepared object to base this prepared statement off of.
     *                      The Statement does not retain any ownership over the CassPrepared object.
     */
    explicit Statement(
        const CassPrepared* cass_prepared
    );

    CassStatementPtr m_cass_statement_ptr{nullptr}; ///< The underlying cassandra prepared statement object.
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-template"
template<typename From, typename To>
static auto bind_list(
    CassStatement* cass_statement,
    size_t position,
    const std::vector<From>& values,
    CassError(*functor)(CassCollection*, To)
) -> bool
{
    CassCollectionPtr cass_list_ptr{cass_collection_new(CASS_COLLECTION_TYPE_LIST, values.size())};

    for(auto& value : values)
    {
        CassError rc = functor(cass_list_ptr.get(), static_cast<To>(value));
        if(rc != CASS_OK)
        {
            return false;
        }
    }

    CassError rc = cass_statement_bind_collection(
        cass_statement,
        position,
        cass_list_ptr.get()
    );

    return (rc == CASS_OK);
}

template<typename From, typename To>
static auto bind_list(
    CassStatement* cass_statement,
    std::string_view name,
    const std::vector<From>& values,
    CassError(*functor)(CassCollection*, To)
) -> bool
{
    CassCollectionPtr cass_list_ptr{cass_collection_new(CASS_COLLECTION_TYPE_LIST, values.size())};

    for(auto& value : values)
    {
        CassError rc = functor(cass_list_ptr.get(), static_cast<To>(value));
        if(rc != CASS_OK)
        {
            return false;
        }
    }

    CassError rc = cass_statement_bind_collection_by_name_n(
        cass_statement,
        name.data(),
        name.length(),
        cass_list_ptr.get()
    );

    return (rc == CASS_OK);
}
#pragma clang diagnostic pop

template<typename T>
auto Statement::BindListTinyInt(
    const std::vector<T>& values,
    size_t position
) -> bool
{
    return bind_list<T, int8_t>(
        m_cass_statement_ptr.get(),
        position,
        values,
        &cass_collection_append_int8
    );
}

template<typename T>
auto Statement::BindListTinyInt(
    const std::vector<T>& values,
    std::string_view name
) -> bool
{
    return bind_list<T, int8_t>(
        m_cass_statement_ptr.get(),
        name,
        values,
        &cass_collection_append_int8
    );
}

} // namespace priam
