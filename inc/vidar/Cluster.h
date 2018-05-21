#pragma once

#include "vidar/ConnectionInfo.h"
#include "vidar/CQL.h"

#include <memory>
#include <chrono>

namespace vidar
{

class Client;
class Cluster;

using namespace std::chrono_literals;

class Cluster
{
    friend Client;
public:

    Cluster() = delete;
    Cluster(const Cluster&) = delete;
    Cluster(Cluster&&) = delete;
    auto operator=(const Cluster&) -> Cluster& = delete;
    auto operator=(Cluster&&) -> Cluster& = delete;

    ~Cluster();

    static auto make(ConnectionInfo connection_info) -> std::unique_ptr<Cluster>;

    auto SetRoundRobinLoadBalancing() -> bool;

    auto SetDatacenterAwareLoadBalancing(
        std::string_view local_dc,
        bool allow_remote_dcs_for_local_consistency_level = false,
        uint64_t used_hosts_per_remote_dc = 2
    ) -> bool;

    auto SetTokenAwareRouting(
        bool enabled
    ) -> bool;

    auto SetLatencyAwareRouting(
        bool enabled,
        double exclusion_threshold,
        std::chrono::milliseconds scale,
        std::chrono::microseconds retry_period,
        std::chrono::milliseconds update_rate,
        uint64_t min_measured
    ) -> bool;

    auto SetHeartbeatInterval(
        std::chrono::seconds interval = 30s,
        std::chrono::seconds idle_timeout = 120s
    ) -> bool;

private:
    ConnectionInfo m_connection_info;

    CassCluster* m_cluster{nullptr};

    explicit Cluster(
        ConnectionInfo connection_info
    );
};

} // namespace vidar
