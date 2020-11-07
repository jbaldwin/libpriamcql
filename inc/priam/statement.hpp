#pragma once

#include "priam/blob.hpp"
#include "priam/cpp_driver.hpp"
#include "priam/list.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace priam
{
class prepared;
class client;

class statement
{
    /**
     * Prepared objects are the only type allowed to create statement's, it needs access to the
     * private constructor.
     */
    friend prepared;

    /**
     * Client uses the underlying cassandra statement object when ExecuteQuery() is called.
     */
    friend client;

public:
    /**
     * Creates an ad-hoc query statement.
     * @param query The full CQL query to execute.
     * @param parameter_count The number of parameters that will be bound to this ad-hoc query.
     *                        Defaults to zero.
     * @return An ad-hoc statement to execute.
     */
    static auto make_statement(std::string_view query, size_t parameter_count = 0) -> std::unique_ptr<statement>;

    statement(const statement&) = delete;
    statement(statement&&)      = delete;
    auto operator=(const statement&) -> statement& = delete;
    auto operator=(statement &&) -> statement& = delete;

    ~statement() = default;

    /**
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_null(size_t position) -> CassError;

    /**
     * @param name Parameter name to bind 'null' to.
     * @return CASS_OK on success.
     */
    auto bind_null(std::string_view name) -> CassError;

    /**
     * @param value Bind this boolean to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_boolean(bool value, size_t position) -> CassError;

    /**
     * @param value Bind this boolean to the prepared statement.
     * @param name Parameter name to bind the boolean to.
     * @return CASS_OK on success.
     */
    auto bind_boolean(bool value, std::string_view name) -> CassError;

    /**
     * @param uuid Bind this UUID to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_uuid(std::string_view uuid, size_t position) -> CassError;

    /**
     * @param uuid Bind this UUID to the prepared statement.
     * @param name Parameter name to bind the UUID to.
     * @return CASS_OK on success.
     */
    auto bind_uuid(std::string_view uuid, std::string_view name) -> CassError;

    /**
     * @param data Text data to bind to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_text(std::string_view data, size_t position) -> CassError;

    /**
     * @param data Text data to bind to the prepared statement.
     * @param name Parameter name to bind the text to.
     * @return CASS_OK on success.
     */
    auto bind_text(std::string_view data, std::string_view name) -> CassError;

    /**
     * @param value Bind this int 8 to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_tiny_int(int8_t value, size_t position) -> CassError;

    /**
     * @param value Bind this int 8 to the prepared statement.
     * @param name Parameter name to bind the int 8 to.
     * @return CASS_OK on success.
     */
    auto bind_tiny_int(int8_t value, std::string_view name) -> CassError;

    /**
     * @param value Bind this int 32 to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_int(int32_t value, size_t position) -> CassError;

    /**
     * @param value Bind this int 32 to the prepared statement.
     * @param name Parameter name to bind the int 32 to.
     * @return CASS_OK on success.
     */
    auto bind_int(int32_t value, std::string_view name) -> CassError;

    /**
     * @param value Bind this int 64 to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_big_int(int64_t value, size_t position) -> CassError;

    /**
     * @param value Bind this int 64 to the prepared statement.
     * @param name Parameter name to bind the int 64 to.
     * @return CASS_OK on success.
     */
    auto bind_big_int(int64_t value, std::string_view name) -> CassError;

    /**
     * @param value Bind this float to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_float(float value, size_t position) -> CassError;

    /**
     * @param value Bind this float to the prepared statement.
     * @param name Parameter name to bind the float to.
     * @return CASS_OK on success.
     */
    auto bind_float(float value, std::string_view name) -> CassError;

    /**
     * @param value Bind this double to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_double(double value, size_t position) -> CassError;

    /**
     * @param value Bind this double to the prepared statement.
     * @param name Parameter name to bind the double to.
     * @return CASS_OK on success.
     */
    auto bind_double(double value, std::string_view name) -> CassError;

    /**
     * @param list Bind this list to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_list(statement_list list, size_t position) -> CassError;

    /**
     * @param list Bind this list to the prepared statement.
     * @param name Parameter name to bind the list to.
     * @return CASS_OK on success.
     */
    auto bind_list(statement_list list, std::string_view name) -> CassError;

    /**
     * @param blob Bind this blob to the prepared statement.
     * @param position The bind position.
     * @return CASS_OK on success.
     */
    auto bind_blob(blob blob, size_t position) -> CassError;

    /**
     * @param blob Bind this blob to the prepared statement.
     * @param name Parameter name to bind the blob to.
     * @return CASS_OK on success.
     */
    auto bind_blob(blob blob, std::string_view name) -> CassError;

private:
    /**
     * Creates a Prepared statement object from the provided underlying cassandra prepared object.
     * @param cass_prepared The underlying cassandra prepared object to base this prepared statement off of.
     *                      The statement does not retain any ownership over the CassPrepared object.
     */
    explicit statement(const CassPrepared* cass_prepared);

    /**
     * Creates an ad-hoc statement object.
     * @param query The raw cql query to be executed.
     * @param parameter_count The number of '?' parameters to bind on this ad-hoc query.
     */
    statement(std::string_view query, size_t parameter_count);

    cass_statement_ptr m_cass_statement_ptr{nullptr}; ///< The underlying cassandra prepared statement object.
};

} // namespace priam
