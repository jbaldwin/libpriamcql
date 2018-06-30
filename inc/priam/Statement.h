#pragma once

#include "priam/CppDriver.h"

#include <string>
#include <string_view>

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
     * @param uuid Bind this UUID to the prepared statement.
     * @param position The bind position.
     * @return True if the uuid was correctly bound.
     */
    auto BindUuid(
        const std::string& uuid,
        size_t position
    ) -> bool;

    /**
     * @param data Text data to bind to the prepared statement.
     * @param position The bind position.
     * @return True if the string was correctly bound.
     */
    auto BindText(
        const std::string& data,
        size_t position
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
     * @param value Bind this int 8 to the prepared statement.
     * @param position The bind position.
     * @return True if the int 8 was correctly bound.
     */
    auto BindTinyInt(
        int8_t value,
        size_t position
    ) -> bool;

    /**
     * @param value Bind this int 32 to the prepared statement.
     * @param position The bind position.
     * @return True if the int 32 was correctly bound.
     */
    auto BindInt32(
        int32_t value,
        size_t position
    ) -> bool;

    /**
     * @param value Bind this int 64 to the prepared statement.
     * @param position The bind position.
     * @return True if the int 64 was correctly bound.
     */
    auto BindInt64(
        int64_t value,
        size_t position
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

} // namespace priam
