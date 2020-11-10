#pragma once

#include "priam/blob.hpp"
#include "priam/cpp_driver.hpp"
#include "priam/list.hpp"
#include "priam/status.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace priam
{
class prepared;
class client;
class statement;

class statement
{
    /// Prepared uses its special private constructor to create statements.
    friend prepared;
    /// Client uses the underlying cassandra statement object when ExecuteQuery() is called.
    friend client;

public:
    /**
     * Creates an ad-hoc statement to be executed.  Use '?' placeholders for locations to bind
     * parameters to.
     * @param query The cql query to be executed.
     */
    explicit statement(std::string_view query);

    statement(const statement&) = delete;
    statement(statement&&)      = default;
    auto operator=(const statement&) -> statement& = delete;
    auto operator=(statement &&) -> statement& = default;

    ~statement() = default;

    /**
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_null(size_t position) -> status;

    /**
     * @param name Parameter name to bind 'null' to.
     * @return CASS_OK on success.
     */
    auto bind_null(std::string_view name) -> status;

    /**
     * @param value Bind this boolean to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_boolean(bool value, size_t position) -> status;

    /**
     * @param value Bind this boolean to the prepared statement.
     * @param name Parameter name to bind the boolean to.
     * @return CASS_OK on success.
     */
    auto bind_boolean(bool value, std::string_view name) -> status;

    /**
     * @param uuid Bind this UUID to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_uuid(uuid uuid, size_t position) -> status;

    /**
     * @param uuid Bind this UUID to the prepared statement.
     * @param name Parameter name to bind the UUID to.
     * @return CASS_OK on success.
     */
    auto bind_uuid(uuid uuid, std::string_view name) -> status;

    /**
     * This function requires a 36 byte string uuid.
     * @param uuid Bind this UUID to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_uuid(std::string_view uuid, size_t position) -> status;

    /**
     * This function requires a 36 byte string uuid.
     * @param uuid Bind this UUID to the prepared statement.
     * @param name Parameter name to bind the UUID to.
     * @return CASS_OK on success.
     */
    auto bind_uuid(std::string_view uuid, std::string_view name) -> status;

    /**
     * @param data Text data to bind to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_text(std::string_view data, size_t position) -> status;

    /**
     * @param data Text data to bind to the prepared statement.
     * @param name Parameter name to bind the text to.
     * @return CASS_OK on success.
     */
    auto bind_text(std::string_view data, std::string_view name) -> status;

    /**
     * @param value Bind this int 8 to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_tiny_int(int8_t value, size_t position) -> status;

    /**
     * @param value Bind this int 8 to the prepared statement.
     * @param name Parameter name to bind the int 8 to.
     * @return CASS_OK on success.
     */
    auto bind_tiny_int(int8_t value, std::string_view name) -> status;

    /**
     * @param value Bind this int 32 to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_int(int32_t value, size_t position) -> status;

    /**
     * @param value Bind this int 32 to the prepared statement.
     * @param name Parameter name to bind the int 32 to.
     * @return CASS_OK on success.
     */
    auto bind_int(int32_t value, std::string_view name) -> status;

    /**
     * @param value Bind this int 64 to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_big_int(int64_t value, size_t position) -> status;

    /**
     * @param value Bind this int 64 to the prepared statement.
     * @param name Parameter name to bind the int 64 to.
     * @return CASS_OK on success.
     */
    auto bind_big_int(int64_t value, std::string_view name) -> status;

    /**
     * @param value Bind this float to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_float(float value, size_t position) -> status;

    /**
     * @param value Bind this float to the prepared statement.
     * @param name Parameter name to bind the float to.
     * @return CASS_OK on success.
     */
    auto bind_float(float value, std::string_view name) -> status;

    /**
     * @param value Bind this double to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_double(double value, size_t position) -> status;

    /**
     * @param value Bind this double to the prepared statement.
     * @param name Parameter name to bind the double to.
     * @return CASS_OK on success.
     */
    auto bind_double(double value, std::string_view name) -> status;

    /**
     * @param list Bind this list to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_list(statement_list list, size_t position) -> status;

    /**
     * @param list Bind this list to the prepared statement.
     * @param name Parameter name to bind the list to.
     * @return CASS_OK on success.
     */
    auto bind_list(statement_list list, std::string_view name) -> status;

    /**
     * @param blob Bind this blob to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_blob(blob blob, size_t position) -> status;

    /**
     * @param blob Bind this blob to the prepared statement.
     * @param name Parameter name to bind the blob to.
     * @return CASS_OK on success.
     */
    auto bind_blob(blob blob, std::string_view name) -> status;

    /**
     * Resets all bound parameters on the statement for another execution.
     */
    auto reset() -> status;

private:
    /**
     * Creates a Prepared statement object from the provided underlying cassandra prepared object.
     * @param cass_prepared The underlying cassandra prepared object to base this prepared statement off of.
     *                      The statement does not retain any ownership over the CassPrepared object.
     * @param parameter_count The number of parameters to bind to this statement.
     */
    explicit statement(const CassPrepared* cass_prepared, size_t parameter_count);

    /// The number of parameters that can be bound to this statement.
    size_t m_parameter_count{0};
    /// The underlying cassandra prepared statement object.
    cass_statement_ptr m_cass_statement_ptr{nullptr};
};

} // namespace priam
