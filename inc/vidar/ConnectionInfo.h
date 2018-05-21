#pragma once

#include <string>
#include <vector>

namespace vidar
{

class ConnectionInfo
{
public:
    ConnectionInfo() = default;
    ConnectionInfo(const ConnectionInfo&) = default;
    ConnectionInfo(ConnectionInfo&&) = default;

    auto operator=(const ConnectionInfo&) -> ConnectionInfo& = default;
    auto operator=(ConnectionInfo&&) -> ConnectionInfo& = default;

    /**
     * @param host Adds the host to the bootstrap c* cluster host list.
     * @return ConnectionInfo
     */
    auto AddHost(std::string host) -> ConnectionInfo&;

    /**
     * @param port Sets the port for the C* cluster.
     * @return ConnectionInfo
     */
    auto SetPort(uint16_t port) -> ConnectionInfo&;

    /**
     * @param username The username to connect to the C* cluster.
     * @return ConnectionInfo
     */
    auto SetUsername(std::string username) -> ConnectionInfo&;

    /**
     * @param password The password to connect to the C* cluster.
     * @return ConnectionInfo
     */
    auto SetPassword(std::string password) -> ConnectionInfo&;

    /**
     * @return Gets a CQL formatted version of the bootstrap C* hosts.
     */
    auto GetFormattedHosts() const -> std::string;

    /**
     * @return Gets the port to connect to the C* servers.
     */
    auto GetPort() const -> uint16_t;

    /**
     * @return Gets the username to connect to the C* servers.
     */
    auto GetUsername() const -> const std::string&;

    /**
     * @return Gets the password to connect to the C* servers.
     */
    auto GetPassword() const -> const std::string&;

private:
    std::vector<std::string> m_hosts;   ///< The set of bootstrap hosts to connect to.
    uint16_t m_port;    ///< The port to connect to.
    std::string m_username; ///< The user name to connect with, can be empty for no user authentication.
    std::string m_password; ///< The password to connect with.  If the username is empty this is ignored.
};

} // namespace vidar
