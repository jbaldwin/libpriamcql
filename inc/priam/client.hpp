#pragma once

#include "priam/Cluster.hpp"
#include "priam/CppDriver.hpp"

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace priam
{
class Result;
class Prepared;
class Statement;

class client
{
    // Access for the underlying cassandra session object.
    friend Prepared;

public:
    /**
     * Creates a Client to connect and execute statements against Cassandra.  The Cluster object defines
     * the Cassandra hosts as well as load balancing and routing settings.
     * @param cluster_ptr Cluster setting information.  Ownership is moved into the Client object.
     * @param connect_timeout The amount of time to wait to connect to the Cassandra servers, defaults to 30 seconds.
     */
    explicit client(
        std::unique_ptr<Cluster> cluster_ptr, std::chrono::milliseconds connect_timeout = std::chrono::seconds{30});

    client(const client&) = delete;
    client(client&&)      = default;
    auto operator=(const client&) -> client& = delete;
    auto operator=(client &&) -> client& = default;

    ~client() = default;

    /**
     * Creates a prepared statement and registers it with the Cassandra cluster this client is connected to.
     * @param name Name to register the prepared statement as.  Can later be fetched by this name.
     * @param query The raw prepared statement with '?' marks for parameter binding.
     * @throw std::runtime_error If the registering of the prepared statement fails.
     * @return A shared ownership with the Client of the Prepared statement object.
     */
    auto prepared_register(std::string name, const std::string& query) -> std::shared_ptr<Prepared>;

    /**
     * Gets a registered prepared statement by name.
     * @param name The registered name of the prepared statement, see prepared_register().
     * @return The registered prepared statement, or nullptr if a prepared statement has not been
     *        registered with 'name'.
     */
    auto prepared_lookup(const std::string& name) -> std::shared_ptr<Prepared>;

    /**
     * Executes the provided statement.  This is asynchronous execution and will return immediately.
     * The OnCompleteCallback is called when the statement's query completes or times out.  This callback
     * is run on one of the various client driver background execution threads, not on the originating thread
     * that called execute_statement.  Beware of race conditions in the callback!
     *
     * @param statement The statement to execute.
     * @param on_complete_callback The callback to execute with the Result on the query completion.
     * @param timeout The timeout for this query.  0 signals no timeout.
     * @param consistency The Cassandra consistency level to use for this query, defaults to LOCAL_ONE.
     */
    auto execute_statement(
        std::unique_ptr<Statement>         statement,
        std::function<void(priam::Result)> on_complete_callback,
        std::chrono::milliseconds          timeout = std::chrono::milliseconds{0},
        CassConsistency                    consistency = CassConsistency::CASS_CONSISTENCY_LOCAL_ONE) -> void;

    /**
     * Executes the provided statement.  THis is synchronous execution and will block until completed
     * or the query times out.
     * @param statement The statement to execute.
     * @param timeout The timeout for this query.  0 signals no timeout.
     * @param consistency The Cassandra consistency level to use for this query.
     * @return The Result of the query completion.
     */
    auto execute_statement(
        std::unique_ptr<Statement> statement,
        std::chrono::milliseconds  timeout     = std::chrono::milliseconds{0},
        CassConsistency            consistency = CassConsistency::CASS_CONSISTENCY_LOCAL_ONE) -> priam::Result;

private:
    /// Cluster settings information.
    std::unique_ptr<Cluster> m_cluster_ptr{nullptr};
    /// Client session information.
    CassSessionPtr m_cass_session_ptr{nullptr};

    /// All registered prepared statements on this client indexed by their name.
    std::unordered_map<std::string, std::shared_ptr<Prepared>> m_prepared_statements{};

    /**
     * Internal callback function that is always registered with the underlying cpp-driver.
     * @param query_future The cassandra query future object.
     * @param data The internal data metadata on the query to turn it into a Result.
     */
    static auto internal_on_complete_callback(CassFuture* query_future, void* data) -> void;
};

} // namespace priam
