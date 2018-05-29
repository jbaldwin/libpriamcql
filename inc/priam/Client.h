#pragma once

#include "priam/CppDriver.h"
#include "priam/Cluster.h"

#include <vector>
#include <memory>
#include <chrono>
#include <functional>

namespace priam
{

class Result;
class Prepared;
class Statement;

using namespace std::chrono_literals;

class Client
{
    /**
     * Access for the underlying cassandra session object.
     */
    friend Prepared;

public:
    /**
     * Creates a Client to connect and execute statements against Cassandra.  The Cluster object defines
     * the Cassandra hosts as well as load balancing and routing settings.
     * @param cluster_ptr Cluster setting information.  Ownership is moved into the Client object.
     * @param connect_timeout The amount of time to wait to connect to the Cassandra servers.
     */
    explicit Client(
        std::unique_ptr<Cluster> cluster_ptr,
        std::chrono::milliseconds connect_timeout = 30s
    );

    Client(const Client&) = delete; ///< No copying
    Client(Client&&) = default;
    auto operator=(const Client&) -> Client& = delete;
    auto operator=(Client&&) -> Client& = default;

    ~Client() = default;

    /**
     * Creates a prepared statement and registers it with the Cassandra cluster this client is connected to.
     * @param query The raw prepared statement with '?' marks for parameter binding.
     * @throws std::runtime_error If the registering of the prepared statement fails.
     * @return A shared ownership with the Client of the Prepared statement object.
     */
    auto CreatePrepared(
        const std::string& query
    ) -> std::shared_ptr<Prepared>;

    /**
     * Executes the provided statement.  This is asynchronous execution and will return immediatly.
     * The OnCompleteCallback is called when the statement's query completes or times out.  This callback
     * is run on one of the various client driver background execution threads, not on the originating thread
     * that called ExecuteStatement.  Beware of race conditions in the callback!
     *
     * @param statement The statement to execute.
     * @param on_complete_callback The callback to execute with the Result on the query completion.
     * @param timeout The timeout for this query.  0 signals no timeout.
     * @param consistency The Cassandra consistency level to use for this query.
     */
    auto ExecuteStatement(
        std::unique_ptr<Statement> statement,
        std::function<void(priam::Result)> on_complete_callback,
        std::chrono::milliseconds timeout = 0ms,
        CassConsistency consistency = CassConsistency::CASS_CONSISTENCY_LOCAL_ONE
    ) -> void;

private:
    std::unique_ptr<Cluster> m_cluster_ptr{nullptr}; ///< Cluster settings information.
    CassSessionPtr m_cass_session_ptr{nullptr};      ///< Client session information.

    std::vector<std::shared_ptr<Prepared>> m_prepared_statements{}; ///< All registered prepared statements on this client.

    /**
     * Internal callback function that is always registered with the underlying cpp-driver.
     * @param query_future The cassandra query future object.
     * @param data The internal data metadata on the query to turn it into a Result.
     */
    static auto internal_on_complete_callback(
        CassFuture* query_future,
        void* data
    ) -> void;
};

} // namespace priam
