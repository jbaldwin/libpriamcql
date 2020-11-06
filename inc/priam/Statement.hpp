#pragma once

#include "priam/Blob.hpp"
#include "priam/CppDriver.hpp"
#include "priam/List.hpp"

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
    Statement(Statement&&)      = delete;
    auto operator=(const Statement&) -> Statement& = delete;
    auto operator=(Statement &&) -> Statement& = delete;

    ~Statement() = default;

    /**
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto BindNull(size_t position) -> CassError;

    /**
     * @param name Parameter name to bind 'null' to.
     * @return CASS_OK on success.
     */
    auto BindNull(std::string_view name) -> CassError;

    /**
     * @param value Bind this boolean to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto BindBoolean(bool value, size_t position) -> CassError;

    /**
     * @param value Bind this boolean to the prepared statement.
     * @param name Parameter name to bind the boolean to.
     * @return CASS_OK on success.
     */
    auto BindBoolean(bool value, std::string_view name) -> CassError;

    /**
     * @param uuid Bind this UUID to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto BindUuid(std::string_view uuid, size_t position) -> CassError;

    /**
     * @param uuid Bind this UUID to the prepared statement.
     * @param name Parameter name to bind the UUID to.
     * @return CASS_OK on success.
     */
    auto BindUuid(std::string_view uuid, std::string_view name) -> CassError;

    /**
     * @param data Text data to bind to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto BindText(std::string_view data, size_t position) -> CassError;

    /**
     * @param data Text data to bind to the prepared statement.
     * @param name Parameter name to bind the text to.
     * @return CASS_OK on success.
     */
    auto BindText(std::string_view data, std::string_view name) -> CassError;

    /**
     * @param value Bind this int 8 to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto BindTinyInt(int8_t value, size_t position) -> CassError;

    /**
     * @param value Bind this int 8 to the prepared statement.
     * @param name Parameter name to bind the int 8 to.
     * @return CASS_OK on success.
     */
    auto BindTinyInt(int8_t value, std::string_view name) -> CassError;

    /**
     * @param value Bind this int 32 to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto BindInt(int32_t value, size_t position) -> CassError;

    /**
     * @param value Bind this int 32 to the prepared statement.
     * @param name Parameter name to bind the int 32 to.
     * @return CASS_OK on success.
     */
    auto BindInt(int32_t value, std::string_view name) -> CassError;

    /**
     * @param value Bind this int 64 to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto BindBigInt(int64_t value, size_t position) -> CassError;

    /**
     * @param value Bind this int 64 to the prepared statement.
     * @param name Parameter name to bind the int 64 to.
     * @return CASS_OK on success.
     */
    auto BindBigInt(int64_t value, std::string_view name) -> CassError;

    /**
     * @param value Bind this float to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto BindFloat(float value, size_t position) -> CassError;

    /**
     * @param value Bind this float to the prepared statement.
     * @param name Parameter name to bind the float to.
     * @return CASS_OK on success.
     */
    auto BindFloat(float value, std::string_view name) -> CassError;

    /**
     * @param value Bind this double to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto BindDouble(double value, size_t position) -> CassError;

    /**
     * @param value Bind this double to the prepared statement.
     * @param name Parameter name to bind the double to.
     * @return CASS_OK on success.
     */
    auto BindDouble(double value, std::string_view name) -> CassError;

    /**
     * @param list Bind this list to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto BindList(StatementList list, size_t position) -> CassError;

    /**
     * @param list Bind this list to the prepared statement.
     * @param name Parameter name to bind the list to.
     * @return CASS_OK on success.
     */
    auto BindList(StatementList list, std::string_view name) -> CassError;

    /**
     * @param blob Bind this blob to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto BindBlob(Blob blob, size_t position) -> CassError;

    /**
     * @param blob Bind this blob to the prepared statement.
     * @param name Parameter name to bind the blob to.
     * @return CASS_OK on success.
     */
    auto BindBlob(Blob blob, std::string_view name) -> CassError;

private:
    /**
     * Creates a Prepared Statement object from the provided underlying cassandra prepared object.
     * @param cass_prepared The underlying cassandra prepared object to base this prepared statement off of.
     *                      The Statement does not retain any ownership over the CassPrepared object.
     */
    explicit Statement(const CassPrepared* cass_prepared);

    CassStatementPtr m_cass_statement_ptr{nullptr}; ///< The underlying cassandra prepared statement object.
};

} // namespace priam
