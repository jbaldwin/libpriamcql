#pragma once

#include "priam/CppDriver.hpp"
#include "priam/List.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace priam {

class Prepared;
class Client;

class Statement {
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
    auto operator=(Statement &&) -> Statement& = delete;

    ~Statement() = default;

    /**
     * @param position The bind position.
     * @return True if 'null' was correctly bound.
     */
    auto BindNull(
        size_t position) -> bool;

    /**
     * @param name Parameter name to bind 'null' to.
     * @return True  if 'null' was correctly bound.
     */
    auto BindNull(
        std::string_view name) -> bool;

    auto BindBoolean(
        bool value,
        size_t position) -> bool;

    auto BindBoolean(
        bool value,
        std::string_view name) -> bool;

    /**
     * @param uuid Bind this UUID to the prepared statement.
     * @param position The bind position.
     * @return True if the UUID was correctly bound.
     */
    auto BindUuid(
        std::string_view uuid,
        size_t position) -> bool;

    /**
     * @param uuid Bind this UUID to the prepared statement.
     * @param name Parameter name to bind the UUID to.
     * @return True if the UUID was correctly bound.
     */
    auto BindUuid(
        std::string_view uuid,
        std::string_view name) -> bool;

    /**
     * @param data Text data to bind to the prepared statement.
     * @param position The bind position.
     * @return True if the string was correctly bound.
     */
    auto BindText(
        std::string_view data,
        size_t position) -> bool;

    /**
     * @param data Text data to bind to the prepared statement.
     * @param name Parameter name to bind the text to.
     * @return True if the text was correctly bound.
     */
    auto BindText(
        std::string_view data,
        std::string_view name) -> bool;

    /**
     * @param value Bind this int 8 to the prepared statement.
     * @param position The bind position.
     * @return True if the int 8 was correctly bound.
     */
    auto BindTinyInt(
        int8_t value,
        size_t position) -> bool;

    /**
     * @param value Bind this int 8 to the prepared statement.
     * @param name Parameter name to bind the int 8 to.
     * @return True if the int 8 was correctly bound.
     */
    auto BindTinyInt(
        int8_t value,
        std::string_view name) -> bool;

    /**
     * @param value Bind this int 32 to the prepared statement.
     * @param position The bind position.
     * @return True if the int 32 was correctly bound.
     */
    auto BindInt(
        int32_t value,
        size_t position) -> bool;

    /**
     * @param value Bind this int 32 to the prepared statement.
     * @param name Parameter name to bind the int 32 to.
     * @return True if the int 32 was correctly bound.
     */
    auto BindInt(
        int32_t value,
        std::string_view name) -> bool;

    /**
     * @param value Bind this int 64 to the prepared statement.
     * @param position The bind position.
     * @return True if the int 64 was correctly bound.
     */
    auto BindBigInt(
        int64_t value,
        size_t position) -> bool;

    /**
     * @param value Bind this int 64 to the prepared statement.
     * @param name Parameter name to bind the int 64 to.
     * @return True if the int 64 was correctly bound.
     */
    auto BindBigInt(
        int64_t value,
        std::string_view name) -> bool;

    /**
     * @param value Bind this float to the prepared statement.
     * @param position The bind position.
     * @return True if the float was correctly bound.
     */
    auto BindFloat(
        float value,
        size_t position) -> bool;

    /**
     * @param value Bind this float to the prepared statement.
     * @param name Parameter name to bind the float to.
     * @return True if the float was correctly bound.
     */
    auto BindFloat(
        float value,
        std::string_view name) -> bool;

    /**
     * @param value Bind this double to the prepared statement.
     * @param position The bind position.
     * @return True if the double was correctly bound.
     */
    auto BindDouble(
        double value,
        size_t position) -> bool;

    /**
     * @param value Bind this double to the prepared statement.
     * @param name Parameter name to bind the double to.
     * @return True if the double was correctly bound.
     */
    auto BindDouble(
        double value,
        std::string_view name) -> bool;

    /**
     * @param list Bind this list to the prepared statement.
     * @param position The bind position.
     * @return True if the list was correctly bound.
     */
    auto BindList(
        StatementList list,
        size_t position) -> bool;

    /**
     * @param list Bind this list to the prepared statement.
     * @param name Parameter name to bind the list to.
     * @return True if the list was correctly bound.
     */
    auto BindList(
        StatementList list,
        std::string_view name) -> bool;

private:
    /**
     * Creates a Prepared Statement object from the provided underlying cassandra prepared object.
     * @param cass_prepared The underlying cassandra prepared object to base this prepared statement off of.
     *                      The Statement does not retain any ownership over the CassPrepared object.
     */
    explicit Statement(
        const CassPrepared* cass_prepared);

    CassStatementPtr m_cass_statement_ptr { nullptr }; ///< The underlying cassandra prepared statement object.
};

} // namespace priam
