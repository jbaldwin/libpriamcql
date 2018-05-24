#pragma once

#include "priam/CppDriver.h"

#include <vector>
#include <memory>
#include <chrono>

namespace priam
{

class Client;
class Cluster;

using namespace std::chrono_literals;

class Cluster
{
    /**
     * Client requires access to the underlying cassandra cluster object when creating the session.
     */
    friend Client;
public:
    Cluster(const Cluster&) = delete;
    Cluster(Cluster&&) = delete;
    auto operator=(const Cluster&) -> Cluster& = delete;
    auto operator=(Cluster&&) -> Cluster& = delete;

    ~Cluster();

    /**
     * This function is used to properly create a Cluster.
     * @return Unique ownership to the cluster.
     */
    static auto make() -> std::unique_ptr<Cluster>;

    /**
     * @param host Adds the host to the bootstrap c* cluster host list.
     * @return Cluster
     */
    auto AddHost(
        std::string host
    ) -> Cluster&;

    /**
     * @param port Sets the port for the C* cluster.
     * @return Cluster
     */
    auto SetPort(
        uint16_t port
    ) -> Cluster&;

    /**
     * @param username The username to connect to the C* cluster.
     * @param password The password to connect to the C* cluster.
     * @return Cluster
     */
    auto SetUsernamePassword(
        std::string_view username,
        std::string_view password
    ) -> Cluster&;

    /**
     * Sets the Cluster to use round robin load balancing policy.
     * @return True if the policy was set.
     */
    auto SetRoundRobinLoadBalancing() -> bool;

    /**
     * Sets the Cluster to use data center aware load balancing policy.
     * @param local_dc The name of the local data center.
     * @param allow_remote_dcs_for_local_consistency_level True if remote data centers should be used in local consistency.
     * @param used_hosts_per_remote_dc The number of hosts to use if remote data centers are allowed.
     * @return True if the policy was set.
     */
    auto SetDatacenterAwareLoadBalancing(
        std::string_view local_dc,
        bool allow_remote_dcs_for_local_consistency_level = false,
        uint64_t used_hosts_per_remote_dc = 2
    ) -> bool;

    /**
     * See cassandra docs on token aware routing.  This in a nutshell sends the query
     * to the appropriate node in the cluster.
     * @param enabled Flag to enable or disable token aware routing.
     * @return True if updated.
     */
    auto SetTokenAwareRouting(
        bool enabled
    ) -> bool;

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
    auto SetLatencyAwareRouting(
        bool enabled,
        double exclusion_threshold,
        std::chrono::milliseconds scale,
        std::chrono::milliseconds retry_period,
        std::chrono::milliseconds update_rate,
        uint64_t min_measured
    ) -> bool;

    /**
     * Sets the heartbeat interval for the hosts in the Cluster to determine if they are still responding.
     * @param interval The time interval to send a heartbeat request.
     * @param idle_timeout The maximum amount of time until a host is marked as down.
     * @return True if the heartbeat intervals were updated.
     */
    auto SetHeartbeatInterval(
        std::chrono::seconds interval = 30s,
        std::chrono::seconds idle_timeout = 120s
    ) -> bool;

private:
    std::vector<std::string> m_hosts;   ///< The set of bootstrap hosts to connect to.

    CassCluster* m_cluster{nullptr};    ///< The underlying cassandra cluster object.

    /**
     * Private constructor to force the use of unique_ptr<Cluster>.
     */
    Cluster();

    /**
     * Sets the bootstrap hosts from the list of hosts added to the cluster.
     * The Client will call this when it acquires the Cluster.
     * @throws std::runtime_error If the hosts couldn't be set on the Cluster.
     */
    auto setBootstrapHosts() -> void;
};

} // namespace priam
