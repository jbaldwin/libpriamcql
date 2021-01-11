#pragma once

#include "priam/cpp_driver.hpp"

#include <chrono>
#include <memory>
#include <set>
#include <string_view>

namespace priam
{
class client;

class cluster
{
    /**
     * Client requires access to the underlying cassandra cluster object when creating the session.
     */
    friend client;

public:
    cluster(const cluster&) = delete;
    cluster(cluster&&)      = delete;
    auto operator=(const cluster&) -> cluster& = delete;
    auto operator=(cluster &&) -> cluster& = delete;

    ~cluster() = default;

    /**
     * This function is used to properly create a Cluster.
     * @return Unique ownership to the cluster.
     */
    static auto make_unique() -> std::unique_ptr<cluster>;

    /**
     * @param host Adds the host to the bootstrap c* cluster host list.
     * @return Cluster
     */
    auto add_host(std::string host) -> cluster&;

    /**
     * @param host Adds a specific whitelist host, hosts not on this list
     *             will not be connected to.
     * @return Cluster
     */
    auto add_whitelist_filtering_host(std::string host) -> cluster&;

    /**
     * @param port Sets the port for the C* cluster.
     * @return Cluster
     */
    auto port(uint16_t port) -> cluster&;

    /**
     * @param username The username to connect to the C* cluster.
     * @param password The password to connect to the C* cluster.
     * @return Cluster
     */
    auto username_and_password(std::string_view username, std::string_view password) -> cluster&;

    /**
     * Sets the Cluster to use round robin load balancing policy.
     * @return True if the policy was set.
     */
    auto round_robin_load_balancing() -> bool;

    /**
     * Sets the Cluster to use data center aware load balancing policy.
     * @param local_dc The name of the local data center.
     * @param allow_remote_dcs_for_local_consistency_level True if remote data centers should be used in local
     * consistency.
     * @param used_hosts_per_remote_dc The number of hosts to use if remote data centers are allowed.
     * @return True if the policy was set.
     */
    auto datacenter_aware_load_balancing(
        std::string_view local_dc,
        bool             allow_remote_dcs_for_local_consistency_level = false,
        uint64_t         used_hosts_per_remote_dc                     = 2) -> bool;

    /**
     * See cassandra docs on token aware routing.  This in a nutshell sends the query
     * to the appropriate node in the cluster.
     * @param enabled Flag to enable or disable token aware routing.
     * @return True if updated.
     */
    auto token_aware_routing(bool enabled) -> bool;

    /**
     * @param enabled Flag to enable or disable latency aware routing.
     * @param exclusion_threshold Controls how much worse the latency must be compared to the
     *                            average latency of the best performing node before it penalized.
     * @param scale Controls the weight given to older latencies when calculating the average
     *              latency of a node. A bigger scale will give more weight to older latency measurements.
     * @param retry_period The amount of time a node is penalized by the policy before
     *                     being given a second chance when the current average latency exceeds the calculated
     *                     threshold (exclusion_threshold * best_average_latency).
     * @param update_rate The rate at  which the best average latency is recomputed.
     * @param min_measured The minimum number of measurements per-host required to
     *                     be considered by the policy.
     * @return True if updated.
     */
    auto latency_aware_routing(
        bool                      enabled,
        double                    exclusion_threshold,
        std::chrono::milliseconds scale,
        std::chrono::milliseconds retry_period,
        std::chrono::milliseconds update_rate,
        uint64_t                  min_measured) -> bool;


    /**
     * Enables constant speculative executions for requests sent to the Cluster.
     * @param delay Controls the length of time before sending a speculative request
     * @param max_executions The maximum number of speculative requests to send.
                             This should not be higher than your max replication factor.
     * @return True if the heartbeat intervals were updated.
     */
    auto cluster::speculative_execution(
        std::chrono::milliseconds delay,
        int                       max_executions) -> bool;

    /**
     * Sets the heartbeat interval for the hosts in the Cluster to determine if they are still responding.
     * @param interval The time interval to send a heartbeat request.
     * @param idle_timeout The maximum amount of time until a host is marked as down.
     * @return True if the heartbeat intervals were updated.
     */
    auto heartbeat_interval(
        std::chrono::seconds interval     = std::chrono::seconds{30},
        std::chrono::seconds idle_timeout = std::chrono::seconds{120}) -> bool;

private:
    /// The underlying cassandra cluster object.
    cass_cluster_ptr m_cass_cluster_ptr{nullptr};
    /// The set of bootstrap hosts to connect to.
    std::set<std::string> m_hosts{};
    /// The set of whitelist hosts to allow this client to connect to.
    std::set<std::string> m_whitelist_hosts{};

    /**
     * Private constructor to force the use of unique_ptr<Cluster>.
     */
    cluster();

    /**
     * Sets the bootstrap hosts from the list of hosts added to the cluster.
     * The Client will call this when it acquires the Cluster.
     * @throws std::runtime_error If the hosts couldn't be set on the Cluster.
     */
    auto bootstrap_hosts() -> void;
};

} // namespace priam
