#pragma once

#include "priam/cpp_driver.hpp"

#include <string>

namespace priam
{
enum class status
{
    ok                            = CASS_OK,
    client_bad_params             = CASS_ERROR_LIB_BAD_PARAMS,             // 1, "Bad parameters")
    client_no_streams             = CASS_ERROR_LIB_NO_STREAMS,             // 2, "No streams available")
    client_unable_to_init         = CASS_ERROR_LIB_UNABLE_TO_INIT,         // 3, "Unable to initialize")
    client_message_encode         = CASS_ERROR_LIB_MESSAGE_ENCODE,         // 4, "Unable to encode message")
    client_host_resolution        = CASS_ERROR_LIB_HOST_RESOLUTION,        // 5, "Unable to resolve host")
    client_unexpected_response    = CASS_ERROR_LIB_UNEXPECTED_RESPONSE,    // 6, "Unexpected response from server")
    client_requst_queue_full      = CASS_ERROR_LIB_REQUEST_QUEUE_FULL,     // 7, "The request queue is full")
    client_no_available_io_thread = CASS_ERROR_LIB_NO_AVAILABLE_IO_THREAD, // 8, "No available IO threads")
    client_write_error            = CASS_ERROR_LIB_WRITE_ERROR,            // 9, "Write error")
    client_no_hosts_available     = CASS_ERROR_LIB_NO_HOSTS_AVAILABLE,     // 10, "No hosts available")
    client_index_out_of_bounds    = CASS_ERROR_LIB_INDEX_OUT_OF_BOUNDS,    // 11, "Index out of bounds")
    client_invalid_item_count     = CASS_ERROR_LIB_INVALID_ITEM_COUNT,     // 12, "Invalid item count")
    client_invalid_value_type     = CASS_ERROR_LIB_INVALID_VALUE_TYPE,     // 13, "Invalid value type")
    client_request_timed_out      = CASS_ERROR_LIB_REQUEST_TIMED_OUT,      // 14, "Request timed out")
    client_unable_to_set_keyspace = CASS_ERROR_LIB_UNABLE_TO_SET_KEYSPACE, // 15, "Unable to set keyspace")
    client_callback_already_set   = CASS_ERROR_LIB_CALLBACK_ALREADY_SET,   // 16, "Callback already set")
    client_invalid_statement_type = CASS_ERROR_LIB_INVALID_STATEMENT_TYPE, // 17, "Invalid statement type")
    client_name_does_not_exists   = CASS_ERROR_LIB_NAME_DOES_NOT_EXIST,    // 18, "No value or column for name")
    client_unable_to_determine_protocol =
        CASS_ERROR_LIB_UNABLE_TO_DETERMINE_PROTOCOL,              // 19, "Unable to find supported protocol version")
    client_null_value                = CASS_ERROR_LIB_NULL_VALUE, // 20, "NULL value specified")
    client_not_implemented           = CASS_ERROR_LIB_NOT_IMPLEMENTED,           // 21, "Not implemented")
    client_unable_to_connect         = CASS_ERROR_LIB_UNABLE_TO_CONNECT,         // 22, "Unable to connect")
    client_unable_to_close           = CASS_ERROR_LIB_UNABLE_TO_CLOSE,           // 23, "Unable to close")
    client_no_paging_state           = CASS_ERROR_LIB_NO_PAGING_STATE,           // 24, "No paging state")
    client_parameter_unset           = CASS_ERROR_LIB_PARAMETER_UNSET,           // 25, "Parameter unset")
    client_invalid_error_result_type = CASS_ERROR_LIB_INVALID_ERROR_RESULT_TYPE, // 26, "Invalid error result type")
    client_invalid_future_type       = CASS_ERROR_LIB_INVALID_FUTURE_TYPE,       // 27, "Invalid future type")
    client_internal_error            = CASS_ERROR_LIB_INTERNAL_ERROR,            // 28, "Internal error")
    client_invalid_custom_type       = CASS_ERROR_LIB_INVALID_CUSTOM_TYPE,       // 29, "Invalid custom type")
    client_invalid_data              = CASS_ERROR_LIB_INVALID_DATA,              // 30, "Invalid data")
    client_not_enough_data           = CASS_ERROR_LIB_NOT_ENOUGH_DATA,           // 31, "Not enough data")
    client_invalid_state             = CASS_ERROR_LIB_INVALID_STATE,             // 32, "Invalid state")
    client_no_custom_payload         = CASS_ERROR_LIB_NO_CUSTOM_PAYLOAD,         // 33, "No custom payload")
    client_execution_profile_invalid =
        CASS_ERROR_LIB_EXECUTION_PROFILE_INVALID,                 // 34, "Invalid execution profile specified")
    client_no_tracing       = CASS_ERROR_LIB_NO_TRACING_ID,       // 35, "No tracing ID")
    server_server_error     = CASS_ERROR_SERVER_SERVER_ERROR,     // 0x0000, "Server error")
    server_protocl_error    = CASS_ERROR_SERVER_PROTOCOL_ERROR,   // 0x000A, "Protocol error")
    server_bad_credentials  = CASS_ERROR_SERVER_BAD_CREDENTIALS,  // 0x0100, "Bad credentials")
    server_unavailable      = CASS_ERROR_SERVER_UNAVAILABLE,      // 0x1000, "Unavailable")
    server_overloaded       = CASS_ERROR_SERVER_OVERLOADED,       // 0x1001, "Overloaded")
    server_is_bootstrapping = CASS_ERROR_SERVER_IS_BOOTSTRAPPING, // 0x1002, "Is bootstrapping")
    server_truncate_error   = CASS_ERROR_SERVER_TRUNCATE_ERROR,   // 0x1003, "Truncate error")
    server_write_timeout    = CASS_ERROR_SERVER_WRITE_TIMEOUT,    // 0x1100, "Write timeout")
    server_read_timeout     = CASS_ERROR_SERVER_READ_TIMEOUT,     // 0x1200, "Read timeout")
    server_read_failure     = CASS_ERROR_SERVER_READ_FAILURE,     // 0x1300, "Read failure")
    server_function_failure = CASS_ERROR_SERVER_FUNCTION_FAILURE, // 0x1400, "Function failure")
    server_write_failure    = CASS_ERROR_SERVER_WRITE_FAILURE,    // 0x1500, "Write failure")
    server_syntax_error     = CASS_ERROR_SERVER_SYNTAX_ERROR,     // 0x2000, "Syntax error")
    server_unauthorized     = CASS_ERROR_SERVER_UNAUTHORIZED,     // 0x2100, "Unauthorized")
    server_invalid_query    = CASS_ERROR_SERVER_INVALID_QUERY,    // 0x2200, "Invalid query")
    server_config_error     = CASS_ERROR_SERVER_CONFIG_ERROR,     // 0x2300, "Configuration error")
    server_already_exists   = CASS_ERROR_SERVER_ALREADY_EXISTS,   // 0x2400, "Already exists")
    server_unprepared       = CASS_ERROR_SERVER_UNPREPARED,       // 0x2500, "Unprepared")
    ssl_invalid_cert        = CASS_ERROR_SSL_INVALID_CERT,        // 1, "Unable to load certificate")
    ssl_invalid_private_key = CASS_ERROR_SSL_INVALID_PRIVATE_KEY, // 2, "Unable to load private key")
    ssl_no_peer_cert        = CASS_ERROR_SSL_NO_PEER_CERT,        // 3, "No peer certificate")
    ssl_invalid_peer_cert   = CASS_ERROR_SSL_INVALID_PEER_CERT,   // 4, "Invalid peer certificate")
    ssl_identity_mismatch   = CASS_ERROR_SSL_IDENTITY_MISMATCH,   // 5, "Certificate does not match host or IP address")
    ssl_protocol_error      = CASS_ERROR_SSL_PROTOCOL_ERROR,      // 6, "Protocol error")
    ssl_connection_closed   = CASS_ERROR_SSL_CLOSED,              // 7, "Connection closed")
};

/**
 * @param s Convert this status into a human readable string.
 * @return Human readable string representation of status value.
 */
auto to_string(status s) -> std::string;

inline auto cass_error_to_status(CassError ce) -> status
{
    return static_cast<status>(ce);
}

} // namespace priam
