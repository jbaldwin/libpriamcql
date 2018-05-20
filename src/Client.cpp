#include "vidar/Client.h"

struct CassFutureCallbackData
{
    CassFutureCallbackData(
        void* user_data,
        vidar::OnCompleteCallback on_complete_callback
    )
        : m_user_data(user_data),
          m_on_complete_callback(on_complete_callback)
    {

    }

    void* m_user_data;
    vidar::OnCompleteCallback m_on_complete_callback;
};

static auto internal_on_complete_callback(
    CassFuture* query_future,
    void* data
) -> void;

namespace vidar
{

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
    OnCompleteCallback on_complete_callback,
    void* data,
    std::chrono::microseconds timeout,
    CassConsistency consistency
) -> void
{
    auto ptr = std::make_unique<CassFutureCallbackData>(data, on_complete_callback);

    cass_statement_set_consistency(statement->m_statement, consistency);

    if(timeout != 0ms)
    {
        cass_statement_set_request_timeout(statement->m_statement, static_cast<cass_uint64_t>(timeout.count()));
    }

    CassFuture* query_future = cass_session_execute(m_session, statement->m_statement);
    cass_future_set_callback(query_future, internal_on_complete_callback, ptr.release());

    /**
     * The driver will have its own internal reference count.  Documents say it is now
     * safe to release the "application" reference to the future after setting the callback.
     */
    cass_future_free(query_future);
}

} // namespace vidar

static auto internal_on_complete_callback(
    CassFuture* query_future,
    void* data
) -> void
{
    auto ptr = std::unique_ptr<CassFutureCallbackData>(static_cast<CassFutureCallbackData*>(data));

    const CassResult* result = cass_future_get_result(query_future);

    auto callback = ptr->m_on_complete_callback;
    if(callback != nullptr)
    {
        callback(vidar::Result(result), ptr->m_user_data);
    }

    if(result != nullptr)
    {
        cass_result_free(result);
    }
}
