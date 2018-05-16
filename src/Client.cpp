#include "vidar/Client.h"

struct CassFutureCallbackData
{
    CassFutureCallbackData(void* user_data, vidar::OnCompleteCallback on_complete_callback)
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
    ConnectionInfo connection_info,
    bool use_token_aware_routing
)
    : m_connection_info(std::move(connection_info))
{

    CassCluster* cluster = cass_cluster_new();

    if(cluster == nullptr)
    {
        throw std::runtime_error("Client: Failed to initialize cassandra cluster.");
    }

    CassSession* session = cass_session_new();

    if(session == nullptr)
    {
        throw std::runtime_error("Client: Failed to initialize cassandra session.");
    }

    if(!m_connection_info.GetUsername().empty())
    {
        auto& username = m_connection_info.GetUsername();
        auto& password = m_connection_info.GetPassword();
        cass_cluster_set_credentials_n(
            cluster,
            username.c_str(),
            username.length(),
            password.c_str(),
            password.length()
        );
    }

    auto contact_hosts = m_connection_info.GetFormattedHosts();

    if(cass_cluster_set_contact_points_n(cluster, contact_hosts.c_str(), contact_hosts.length()) != CASS_OK)
    {
        throw std::runtime_error("Client: Failed to initialize bootstrap contact hosts: " + contact_hosts);
    }

    if(cass_cluster_set_port(cluster, m_connection_info.GetPort()) != CASS_OK)
    {
        throw std::runtime_error("Client: Failed to initialize port: " + std::to_string(m_connection_info.GetPort()));
    }

    cass_cluster_set_token_aware_routing(cluster, static_cast<cass_bool_t>(use_token_aware_routing));

    CassFuture* connect_future = cass_session_connect(session, cluster);

    // Blocks until connected
    CassError rc = cass_future_error_code(connect_future);
    if(rc == CassError::CASS_OK)
    {
        m_cluster = cluster;
        m_session = session;
        cass_future_free(connect_future);
    }
    else
    {
        const char* message;
        size_t message_length;
        cass_future_error_message(connect_future, &message, &message_length);

        std::string error_msg{message, message_length};

        cass_future_free(connect_future);
        cass_session_free(session);
        cass_cluster_free(cluster);

        throw std::runtime_error("Client: Failed to connect to the cassandra cluster: " + error_msg);
    }
}

Client::~Client()
{
    if(m_session != nullptr)
    {
        CassFuture* session_future = cass_session_close(m_session);

        cass_duration_t timeout_ms = 30'000;
        cass_future_wait_timed(session_future, timeout_ms);

        cass_future_free(session_future);
        cass_session_free(m_session);
        m_session = nullptr;
    }

    if(m_cluster != nullptr)
    {
        cass_cluster_free(m_cluster);
        m_cluster = nullptr;
    }
}

auto Client::CreatePrepared(const std::string& query) -> std::shared_ptr<Prepared>
{
    auto prepared_ptr = std::make_shared<Prepared>(query);
    m_prepared_statements.emplace_back(prepared_ptr);
    return prepared_ptr;
}

auto Client::ExecuteStatement(
    std::unique_ptr<Statement> statement,
    void* data,
    OnCompleteCallback on_complete_callback,
    std::chrono::microseconds timeout
) -> void
{
    auto ptr = std::make_unique<CassFutureCallbackData>(data, on_complete_callback);

    // TODO: support consistency levels
    cass_statement_set_consistency(statement->m_statement, CASS_CONSISTENCY_ONE);
    cass_statement_set_request_timeout(statement->m_statement, static_cast<cass_uint64_t>(timeout.count()));
    CassFuture* query_future = cass_session_execute(m_session, statement->m_statement);
    cass_future_set_callback(query_future, internal_on_complete_callback, ptr.release());
}

} // namespace vidar

static auto internal_on_complete_callback(
    CassFuture* query_future,
    void* data
) -> void
{
    auto ptr = std::unique_ptr<CassFutureCallbackData>(static_cast<CassFutureCallbackData*>(data));

    const CassResult* result = cass_future_get_result(query_future);
    cass_future_free(query_future); // must be alive long enough to grab the result

    auto callback = ptr->m_on_complete_callback;
    if(callback != nullptr)
    {
        // TODO: flesh into a parsed result
        callback(vidar::Result{}, ptr->m_user_data);
    }

    if(result != nullptr)
    {
        cass_result_free(result);
    }
}
