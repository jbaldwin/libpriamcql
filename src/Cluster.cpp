#include "priam/Cluster.h"

#include <sstream>

namespace priam
{

auto Cluster::make() -> std::unique_ptr<Cluster>
{
    return std::unique_ptr<Cluster>(new Cluster());
}

auto Cluster::AddHost(
    std::string host
) -> Cluster&
{
    m_hosts.emplace_back(std::move(host));
    return *this;
}

auto Cluster::SetPort(
    uint16_t port
) -> Cluster&
{
    if(cass_cluster_set_port(m_cluster, port) != CASS_OK)
    {
        throw std::runtime_error("Client: Failed to initialize port: " + std::to_string(port));
    }
    return *this;
}

auto Cluster::SetUsernamePassword(
    std::string_view username,
    std::string_view password
) -> Cluster&
{
    cass_cluster_set_credentials_n(
        m_cluster,
        username.data(),
        username.length(),
        password.data(),
        password.length()
    );
    return *this;
}

Cluster::~Cluster()
{
    if(m_cluster != nullptr)
    {
        cass_cluster_free(m_cluster);
        m_cluster = nullptr;
    }
}

auto Cluster::SetRoundRobinLoadBalancing() -> bool
{
    if(m_cluster != nullptr)
    {
        cass_cluster_set_load_balance_round_robin(m_cluster);
    }
    return false;
}

auto Cluster::SetDatacenterAwareLoadBalancing(
    std::string_view local_dc,
    bool allow_remote_dcs_for_local_consistency_level,
    uint64_t used_hosts_per_remote_dc
) -> bool
{
    if(m_cluster != nullptr)
    {
        CassError error = cass_cluster_set_load_balance_dc_aware_n(
            m_cluster,
            local_dc.data(),
            local_dc.size(),
            static_cast<unsigned>(used_hosts_per_remote_dc),
            static_cast<cass_bool_t>(allow_remote_dcs_for_local_consistency_level)
        );
        return (error == CassError::CASS_OK);
    }
    return false;
}

auto Cluster::SetTokenAwareRouting(
    bool enabled
) -> bool
{
    if(m_cluster != nullptr)
    {
        cass_cluster_set_token_aware_routing(m_cluster, static_cast<cass_bool_t>(enabled));
        return true;
    }
    return false;
}

auto Cluster::SetLatencyAwareRouting(
    bool enabled,
    double exclusion_threshold,
    std::chrono::milliseconds scale,
    std::chrono::milliseconds retry_period,
    std::chrono::milliseconds update_rate,
    uint64_t min_measured
) -> bool
{
    if(m_cluster != nullptr)
    {
        cass_cluster_set_latency_aware_routing(m_cluster, static_cast<cass_bool_t>(enabled));
        if(enabled)
        {
            cass_cluster_set_latency_aware_routing_settings(
                m_cluster,
                exclusion_threshold,
                static_cast<cass_uint64_t>(scale.count()),
                static_cast<cass_uint64_t>(retry_period.count()),
                static_cast<cass_uint64_t>(update_rate.count()),
                min_measured
            );
        }
        return true;
    }
    return false;
}

auto Cluster::SetHeartbeatInterval(
    std::chrono::seconds interval,
    std::chrono::seconds idle_timeout
) -> bool
{
    if(m_cluster != nullptr)
    {
        cass_cluster_set_connection_heartbeat_interval(m_cluster, static_cast<unsigned>(interval.count()));
        cass_cluster_set_connection_idle_timeout(m_cluster, static_cast<unsigned>(idle_timeout.count()));
        return true;
    }
    return false;
}

Cluster::Cluster()
    : m_cluster(cass_cluster_new())
{
    if(m_cluster == nullptr)
    {
        throw std::runtime_error("Client: Failed to initialize cassandra cluster.");
    }
}

auto Cluster::setBootstrapHosts() -> void
{
    std::stringstream ss;

    size_t idx = 0;
    for(auto& host : m_hosts)
    {
        ++idx;
        ss << host;
        if(idx < m_hosts.size())
        {
            ss << ",";
        }
    }

    auto contact_hosts = ss.str();

    if(cass_cluster_set_contact_points_n(m_cluster, contact_hosts.c_str(), contact_hosts.length()) != CASS_OK)
    {
        throw std::runtime_error("Client: Failed to initialize bootstrap contact hosts: " + contact_hosts);
    }
}

} // namespace priam
