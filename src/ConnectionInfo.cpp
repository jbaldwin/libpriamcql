#include "vidar/ConnectionInfo.h"

#include <sstream>

namespace vidar
{

auto ConnectionInfo::AddHost(std::string host) -> ConnectionInfo&
{
    m_hosts.emplace_back(std::move(host));
    return *this;
}

auto ConnectionInfo::SetPort(uint16_t port) -> ConnectionInfo&
{
    m_port = port;
    return *this;
}

auto ConnectionInfo::SetUsername(std::string username) -> ConnectionInfo&
{
    m_username = std::move(username);
    return *this;
}

auto ConnectionInfo::SetPassword(std::string password) -> ConnectionInfo&
{
    m_password = std::move(password);
    return *this;
}

auto ConnectionInfo::GetFormattedHosts() const -> std::string
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

    return ss.str();
}

auto ConnectionInfo::GetPort() const -> uint16_t
{
    return m_port;
}

auto ConnectionInfo::GetUsername() const -> const std::string&
{
    return m_username;
}

auto ConnectionInfo::GetPassword() const -> const std::string&
{
    return m_password;
}

} // namespace vidar
