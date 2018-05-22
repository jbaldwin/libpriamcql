#include "vidar/Client.h"

namespace vidar
{

static const std::string CASS_OK_STR = "CASS_OK";
static const std::string CASS_ERROR_LIB_BAD_PARAMS_STR = "CASS_ERROR_LIB_BAD_PARAMS";
static const std::string CASS_ERROR_LIB_NO_STREAMS_STR = "CASS_ERROR_LIB_NO_STREAMS";
static const std::string CASS_ERROR_LIB_UNABLE_TO_INIT_STR = "CASS_ERROR_LIB_UNABLE_TO_INIT";
static const std::string CASS_ERROR_LIB_MESSAGE_ENCODE_STR = "CASS_ERROR_LIB_MESSAGE_ENCODE";
static const std::string CASS_ERROR_LIB_HOST_RESOLUTION_STR = "CASS_ERROR_LIB_HOST_RESOLUTION";
static const std::string CASS_ERROR_LIB_UNEXPECTED_RESPONSE_STR = "CASS_ERROR_LIB_UNEXPECTED_RESPONSE";
static const std::string CASS_ERROR_LIB_REQUEST_QUEUE_FULL_STR = "CASS_ERROR_LIB_REQUEST_QUEUE_FULL";
static const std::string CASS_ERROR_LIB_NO_AVAILABLE_IO_THREAD_STR = "CASS_ERROR_LIB_NO_AVAILABLE_IO_THREAD";
static const std::string CASS_ERROR_LIB_WRITE_ERROR_STR = "CASS_ERROR_LIB_WRITE_ERROR";
static const std::string CASS_ERROR_LIB_NO_HOSTS_AVAILABLE_STR = "CASS_ERROR_LIB_NO_HOSTS_AVAILABLE";
static const std::string CASS_ERROR_LIB_INDEX_OUT_OF_BOUNDS_STR = "CASS_ERROR_LIB_INDEX_OUT_OF_BOUNDS";
static const std::string CASS_ERROR_LIB_INVALID_ITEM_COUNT_STR = "CASS_ERROR_LIB_INVALID_ITEM_COUNT";
static const std::string CASS_ERROR_LIB_INVALID_VALUE_TYPE_STR = "CASS_ERROR_LIB_INVALID_VALUE_TYPE";
static const std::string CASS_ERROR_LIB_REQUEST_TIMED_OUT_STR = "CASS_ERROR_LIB_REQUEST_TIMED_OUT";
static const std::string CASS_ERROR_LIB_UNABLE_TO_SET_KEYSPACE_STR = "CASS_ERROR_LIB_UNABLE_TO_SET_KEYSPACE";
static const std::string CASS_ERROR_LIB_CALLBACK_ALREADY_SET_STR = "CASS_ERROR_LIB_CALLBACK_ALREADY_SET";
static const std::string CASS_ERROR_LIB_INVALID_STATEMENT_TYPE_STR = "CASS_ERROR_LIB_INVALID_STATEMENT_TYPE";
static const std::string CASS_ERROR_LIB_NAME_DOES_NOT_EXIST_STR = "CASS_ERROR_LIB_NAME_DOES_NOT_EXIST";
static const std::string CASS_ERROR_LIB_UNABLE_TO_DETERMINE_PROTOCOL_STR = "CASS_ERROR_LIB_UNABLE_TO_DETERMINE_PROTOCOL";
static const std::string CASS_ERROR_LIB_NULL_VALUE_STR = "CASS_ERROR_LIB_NULL_VALUE";
static const std::string CASS_ERROR_LIB_NOT_IMPLEMENTED_STR = "CASS_ERROR_LIB_NOT_IMPLEMENTED";
static const std::string CASS_ERROR_LIB_UNABLE_TO_CONNECT_STR = "CASS_ERROR_LIB_UNABLE_TO_CONNECT";
static const std::string CASS_ERROR_LIB_UNABLE_TO_CLOSE_STR = "CASS_ERROR_LIB_UNABLE_TO_CLOSE";
static const std::string CASS_ERROR_LIB_NO_PAGING_STATE_STR = "CASS_ERROR_LIB_NO_PAGING_STATE";
static const std::string CASS_ERROR_LIB_PARAMETER_UNSET_STR = "CASS_ERROR_LIB_PARAMETER_UNSET";
static const std::string CASS_ERROR_LIB_INVALID_ERROR_RESULT_TYPE_STR = "CASS_ERROR_LIB_INVALID_ERROR_RESULT_TYPE";
static const std::string CASS_ERROR_LIB_INVALID_FUTURE_TYPE_STR = "CASS_ERROR_LIB_INVALID_FUTURE_TYPE";
static const std::string CASS_ERROR_LIB_INTERNAL_ERROR_STR = "CASS_ERROR_LIB_INTERNAL_ERROR";
static const std::string CASS_ERROR_LIB_INVALID_CUSTOM_TYPE_STR = "CASS_ERROR_LIB_INVALID_CUSTOM_TYPE";
static const std::string CASS_ERROR_LIB_INVALID_DATA_STR = "CASS_ERROR_LIB_INVALID_DATA";
static const std::string CASS_ERROR_LIB_NOT_ENOUGH_DATA_STR = "CASS_ERROR_LIB_NOT_ENOUGH_DATA";
static const std::string CASS_ERROR_LIB_INVALID_STATE_STR = "CASS_ERROR_LIB_INVALID_STATE";
static const std::string CASS_ERROR_LIB_NO_CUSTOM_PAYLOAD_STR = "CASS_ERROR_LIB_NO_CUSTOM_PAYLOAD";
static const std::string CASS_ERROR_SERVER_SERVER_ERROR_STR = "CASS_ERROR_SERVER_SERVER_ERROR";
static const std::string CASS_ERROR_SERVER_PROTOCOL_ERROR_STR = "CASS_ERROR_SERVER_PROTOCOL_ERROR";
static const std::string CASS_ERROR_SERVER_BAD_CREDENTIALS_STR = "CASS_ERROR_SERVER_BAD_CREDENTIALS";
static const std::string CASS_ERROR_SERVER_UNAVAILABLE_STR = "CASS_ERROR_SERVER_UNAVAILABLE";
static const std::string CASS_ERROR_SERVER_OVERLOADED_STR = "CASS_ERROR_SERVER_OVERLOADED";
static const std::string CASS_ERROR_SERVER_IS_BOOTSTRAPPING_STR = "CASS_ERROR_SERVER_IS_BOOTSTRAPPING";
static const std::string CASS_ERROR_SERVER_TRUNCATE_ERROR_STR = "CASS_ERROR_SERVER_TRUNCATE_ERROR";
static const std::string CASS_ERROR_SERVER_WRITE_TIMEOUT_STR = "CASS_ERROR_SERVER_WRITE_TIMEOUT";
static const std::string CASS_ERROR_SERVER_READ_TIMEOUT_STR = "CASS_ERROR_SERVER_READ_TIMEOUT";
static const std::string CASS_ERROR_SERVER_READ_FAILURE_STR = "CASS_ERROR_SERVER_READ_FAILURE";
static const std::string CASS_ERROR_SERVER_FUNCTION_FAILURE_STR = "CASS_ERROR_SERVER_FUNCTION_FAILURE";
static const std::string CASS_ERROR_SERVER_WRITE_FAILURE_STR = "CASS_ERROR_SERVER_WRITE_FAILURE";
static const std::string CASS_ERROR_SERVER_SYNTAX_ERROR_STR = "CASS_ERROR_SERVER_SYNTAX_ERROR";
static const std::string CASS_ERROR_SERVER_UNAUTHORIZED_STR = "CASS_ERROR_SERVER_UNAUTHORIZED";
static const std::string CASS_ERROR_SERVER_INVALID_QUERY_STR = "CASS_ERROR_SERVER_INVALID_QUERY";
static const std::string CASS_ERROR_SERVER_CONFIG_ERROR_STR = "CASS_ERROR_SERVER_CONFIG_ERROR";
static const std::string CASS_ERROR_SERVER_ALREADY_EXISTS_STR = "CASS_ERROR_SERVER_ALREADY_EXISTS";
static const std::string CASS_ERROR_SERVER_UNPREPARED_STR = "CASS_ERROR_SERVER_UNPREPARED";
static const std::string CASS_ERROR_SSL_INVALID_CERT_STR = "CASS_ERROR_SSL_INVALID_CERT";
static const std::string CASS_ERROR_SSL_INVALID_PRIVATE_KEY_STR = "CASS_ERROR_SSL_INVALID_PRIVATE_KEY";
static const std::string CASS_ERROR_SSL_NO_PEER_CERT_STR = "CASS_ERROR_SSL_NO_PEER_CERT";
static const std::string CASS_ERROR_SSL_INVALID_PEER_CERT_STR = "CASS_ERROR_SSL_INVALID_PEER_CERT";
static const std::string CASS_ERROR_SSL_IDENTITY_MISMATCH_STR = "CASS_ERROR_SSL_IDENTITY_MISMATCH";
static const std::string CASS_ERROR_SSL_PROTOCOL_ERROR_STR = "CASS_ERROR_SSL_PROTOCOL_ERROR";
static const std::string CASS_ERROR_LAST_ENTRY_STR = "CASS_ERROR_LAST_ENTRY";

auto to_string(CassError ce) -> const std::string&
{
    switch (ce)
    {
        case CASS_OK:
            return CASS_OK_STR;
        case CASS_ERROR_LIB_BAD_PARAMS:
            return CASS_ERROR_LIB_BAD_PARAMS_STR;
        case CASS_ERROR_LIB_NO_STREAMS:
            return CASS_ERROR_LIB_NO_STREAMS_STR;
        case CASS_ERROR_LIB_UNABLE_TO_INIT:
            return CASS_ERROR_LIB_UNABLE_TO_INIT_STR;
        case CASS_ERROR_LIB_MESSAGE_ENCODE:
            return CASS_ERROR_LIB_MESSAGE_ENCODE_STR;
        case CASS_ERROR_LIB_HOST_RESOLUTION:
            return CASS_ERROR_LIB_HOST_RESOLUTION_STR;
        case CASS_ERROR_LIB_UNEXPECTED_RESPONSE:
            return CASS_ERROR_LIB_UNEXPECTED_RESPONSE_STR;
        case CASS_ERROR_LIB_REQUEST_QUEUE_FULL:
            return CASS_ERROR_LIB_REQUEST_QUEUE_FULL_STR;
        case CASS_ERROR_LIB_NO_AVAILABLE_IO_THREAD:
            return CASS_ERROR_LIB_NO_AVAILABLE_IO_THREAD_STR;
        case CASS_ERROR_LIB_WRITE_ERROR:
            return CASS_ERROR_LIB_WRITE_ERROR_STR;
        case CASS_ERROR_LIB_NO_HOSTS_AVAILABLE:
            return CASS_ERROR_LIB_NO_HOSTS_AVAILABLE_STR;
        case CASS_ERROR_LIB_INDEX_OUT_OF_BOUNDS:
            return CASS_ERROR_LIB_INDEX_OUT_OF_BOUNDS_STR;
        case CASS_ERROR_LIB_INVALID_ITEM_COUNT:
            return CASS_ERROR_LIB_INVALID_ITEM_COUNT_STR;
        case CASS_ERROR_LIB_INVALID_VALUE_TYPE:
            return CASS_ERROR_LIB_INVALID_VALUE_TYPE_STR;
        case CASS_ERROR_LIB_REQUEST_TIMED_OUT:
            return CASS_ERROR_LIB_REQUEST_TIMED_OUT_STR;
        case CASS_ERROR_LIB_UNABLE_TO_SET_KEYSPACE:
            return CASS_ERROR_LIB_UNABLE_TO_SET_KEYSPACE_STR;
        case CASS_ERROR_LIB_CALLBACK_ALREADY_SET:
            return CASS_ERROR_LIB_CALLBACK_ALREADY_SET_STR;
        case CASS_ERROR_LIB_INVALID_STATEMENT_TYPE:
            return CASS_ERROR_LIB_INVALID_STATEMENT_TYPE_STR;
        case CASS_ERROR_LIB_NAME_DOES_NOT_EXIST:
            return CASS_ERROR_LIB_NAME_DOES_NOT_EXIST_STR;
        case CASS_ERROR_LIB_UNABLE_TO_DETERMINE_PROTOCOL:
            return CASS_ERROR_LIB_UNABLE_TO_DETERMINE_PROTOCOL_STR;
        case CASS_ERROR_LIB_NULL_VALUE:
            return CASS_ERROR_LIB_NULL_VALUE_STR;
        case CASS_ERROR_LIB_NOT_IMPLEMENTED:
            return CASS_ERROR_LIB_NOT_IMPLEMENTED_STR;
        case CASS_ERROR_LIB_UNABLE_TO_CONNECT:
            return CASS_ERROR_LIB_UNABLE_TO_CONNECT_STR;
        case CASS_ERROR_LIB_UNABLE_TO_CLOSE:
            return CASS_ERROR_LIB_UNABLE_TO_CLOSE_STR;
        case CASS_ERROR_LIB_NO_PAGING_STATE:
            return CASS_ERROR_LIB_NO_PAGING_STATE_STR;
        case CASS_ERROR_SERVER_SERVER_ERROR:
            return CASS_ERROR_SERVER_SERVER_ERROR_STR;
        case CASS_ERROR_SERVER_PROTOCOL_ERROR:
            return CASS_ERROR_SERVER_PROTOCOL_ERROR_STR;
        case CASS_ERROR_SERVER_BAD_CREDENTIALS:
            return CASS_ERROR_SERVER_BAD_CREDENTIALS_STR;
        case CASS_ERROR_SERVER_UNAVAILABLE:
            return CASS_ERROR_SERVER_UNAVAILABLE_STR;
        case CASS_ERROR_SERVER_OVERLOADED:
            return CASS_ERROR_SERVER_OVERLOADED_STR;
        case CASS_ERROR_SERVER_IS_BOOTSTRAPPING:
            return CASS_ERROR_SERVER_IS_BOOTSTRAPPING_STR;
        case CASS_ERROR_SERVER_TRUNCATE_ERROR:
            return CASS_ERROR_SERVER_TRUNCATE_ERROR_STR;
        case CASS_ERROR_SERVER_WRITE_TIMEOUT:
            return CASS_ERROR_SERVER_WRITE_TIMEOUT_STR;
        case CASS_ERROR_SERVER_READ_TIMEOUT:
            return CASS_ERROR_SERVER_READ_TIMEOUT_STR;
        case CASS_ERROR_SERVER_SYNTAX_ERROR:
            return CASS_ERROR_SERVER_SYNTAX_ERROR_STR;
        case CASS_ERROR_SERVER_UNAUTHORIZED:
            return CASS_ERROR_SERVER_UNAUTHORIZED_STR;
        case CASS_ERROR_SERVER_INVALID_QUERY:
            return CASS_ERROR_SERVER_INVALID_QUERY_STR;
        case CASS_ERROR_SERVER_CONFIG_ERROR:
            return CASS_ERROR_SERVER_CONFIG_ERROR_STR;
        case CASS_ERROR_SERVER_ALREADY_EXISTS:
            return CASS_ERROR_SERVER_ALREADY_EXISTS_STR;
        case CASS_ERROR_SERVER_UNPREPARED:
            return CASS_ERROR_SERVER_UNPREPARED_STR;
        case CASS_ERROR_SSL_INVALID_CERT:
            return CASS_ERROR_SSL_INVALID_CERT_STR;
        case CASS_ERROR_SSL_INVALID_PRIVATE_KEY:
            return CASS_ERROR_SSL_INVALID_PRIVATE_KEY_STR;
        case CASS_ERROR_SSL_NO_PEER_CERT:
            return CASS_ERROR_SSL_NO_PEER_CERT_STR;
        case CASS_ERROR_SSL_INVALID_PEER_CERT:
            return CASS_ERROR_SSL_INVALID_PEER_CERT_STR;
        case CASS_ERROR_SSL_IDENTITY_MISMATCH:
            return CASS_ERROR_SSL_IDENTITY_MISMATCH_STR;
        case CASS_ERROR_SSL_PROTOCOL_ERROR:
            return CASS_ERROR_SSL_PROTOCOL_ERROR_STR;
        case CASS_ERROR_LAST_ENTRY:
            return CASS_ERROR_LAST_ENTRY_STR;
        case CASS_ERROR_LIB_PARAMETER_UNSET:
            return CASS_ERROR_LIB_PARAMETER_UNSET_STR;
        case CASS_ERROR_LIB_INVALID_ERROR_RESULT_TYPE:
            return CASS_ERROR_LIB_INVALID_ERROR_RESULT_TYPE_STR;
        case CASS_ERROR_LIB_INVALID_FUTURE_TYPE:
            return CASS_ERROR_LIB_INVALID_FUTURE_TYPE_STR;
        case CASS_ERROR_LIB_INTERNAL_ERROR:
            return CASS_ERROR_LIB_INTERNAL_ERROR_STR;
        case CASS_ERROR_LIB_INVALID_CUSTOM_TYPE:
            return CASS_ERROR_LIB_INVALID_CUSTOM_TYPE_STR;
        case CASS_ERROR_LIB_INVALID_DATA:
            return CASS_ERROR_LIB_INVALID_DATA_STR;
        case CASS_ERROR_LIB_NOT_ENOUGH_DATA:
            return CASS_ERROR_LIB_NOT_ENOUGH_DATA_STR;
        case CASS_ERROR_LIB_INVALID_STATE:
            return CASS_ERROR_LIB_INVALID_STATE_STR;
        case CASS_ERROR_LIB_NO_CUSTOM_PAYLOAD:
            return CASS_ERROR_LIB_NO_CUSTOM_PAYLOAD_STR;
        case CASS_ERROR_SERVER_READ_FAILURE:
            return CASS_ERROR_SERVER_READ_FAILURE_STR;
        case CASS_ERROR_SERVER_FUNCTION_FAILURE:
            return CASS_ERROR_SERVER_FUNCTION_FAILURE_STR;
        case CASS_ERROR_SERVER_WRITE_FAILURE:
            return CASS_ERROR_SERVER_WRITE_FAILURE_STR;
    }
}

Client::Client(
    std::unique_ptr<Cluster> cluster_ptr,
    std::chrono::milliseconds connect_timeout
)
    : m_cluster_ptr(std::move(cluster_ptr)),
      m_session(cass_session_new())
{
    if(m_session == nullptr)
    {
        throw std::runtime_error("Client: Failed to initialize cassandra session.");
    }

    /**
     * The Cluster aggregates hosts via "AddHost()", now that the Client owns the Cluster
     * tell the Cluster to bind all the bootstrap hosts to the cassandra cluster object.
     */
    m_cluster_ptr->setBootstrapHosts();

    CassFuture* connect_future = cass_session_connect(m_session, m_cluster_ptr->m_cluster);

    // Common cleanup code to free resources in the event the connection fails.
    auto cleanup = [&]() {
        cass_future_free(connect_future);
        cass_session_free(m_session);
        m_cluster_ptr = nullptr; // free cluster as well
    };

    // cass_future_wait_timed returns false on a timeout, so invert to get a "timed_out" bool.
    auto timed_out = !cass_future_wait_timed(
        connect_future,
        static_cast<cass_duration_t>(std::chrono::duration_cast<std::chrono::microseconds>(connect_timeout).count())
    );

    if(timed_out)
    {
        cleanup();

        std::string error_msg = "Client: Timed out attempting to connect to cassandra with timeout of: ";
        error_msg.append(std::to_string(connect_timeout.count()));
        error_msg.append(" ms.");
        throw std::runtime_error(error_msg);
    }

    // If the connect didn't time out check if there was an error.
    CassError rc = cass_future_error_code(connect_future);
    if(rc == CassError::CASS_OK)
    {
        cass_future_free(connect_future);
    }
    else
    {
        const char* message;
        size_t message_length;
        cass_future_error_message(connect_future, &message, &message_length);

        std::string error_msg{message, message_length};

        // Cleanup after getting the error message out of the future.
        cleanup();

        throw std::runtime_error("Client: Failed to connect to the cassandra cluster: " + error_msg);
    }
}

auto Client::client_move(Client& to, Client& from) noexcept -> void
{
    to.m_cluster_ptr = std::move(from.m_cluster_ptr);
    to.m_session = from.m_session;
    to.m_prepared_statements = std::move(from.m_prepared_statements);

    from.m_session = nullptr;
}

Client::Client(Client&& other) noexcept
{
    Client::client_move(*this, other);
}

auto Client::operator=(Client&& other) noexcept -> Client&
{
    Client::client_move(*this, other);
    return *this;
}

Client::~Client()
{
    if(m_session != nullptr)
    {
        CassFuture* session_future = cass_session_close(m_session);

        std::chrono::milliseconds timeout = 30s;
        auto timed_out = !cass_future_wait_timed(
            session_future,
            static_cast<cass_duration_t>(std::chrono::duration_cast<std::chrono::microseconds>(timeout).count())
        );

        (void)timed_out; // don't want to throw but resources are going to get dropped here..

        cass_future_free(session_future);
        cass_session_free(m_session);
        m_session = nullptr;
    }
}

auto Client::CreatePrepared(
    const std::string& query
) -> std::shared_ptr<Prepared>
{
    // Using new shared_ptr as Prepared's constructor is private but friended to Client.
    auto prepared_ptr = std::shared_ptr<Prepared>(new Prepared(*this, query));
    m_prepared_statements.emplace_back(prepared_ptr);
    return prepared_ptr;
}

auto Client::ExecuteStatement(
    std::unique_ptr<Statement> statement,
    std::function<void(Result)> on_complete_callback,
    std::chrono::milliseconds timeout,
    CassConsistency consistency
) -> void
{
    auto callback_ptr = std::make_unique<std::function<void(Result)>>(std::move(on_complete_callback));

    cass_statement_set_consistency(statement->m_cass_statement, consistency);

    if(timeout != 0ms)
    {
        cass_statement_set_request_timeout(statement->m_cass_statement, static_cast<cass_uint64_t>(timeout.count()));
    }

    CassFuture* query_future = cass_session_execute(m_session, statement->m_cass_statement);
    cass_future_set_callback(query_future, internal_on_complete_callback, callback_ptr.release());
}

auto Client::internal_on_complete_callback(
    CassFuture* query_future,
    void* data
) -> void
{
    auto callback_ptr = std::unique_ptr<std::function<void(Result)>>(static_cast<std::function<void(Result)>*>(data));
    (*callback_ptr)(vidar::Result(query_future));
}

} // namespace vidar
