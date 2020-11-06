#pragma once

#include "priam/CppDriver.hpp"
#include "priam/Statement.hpp"

#include <memory>
#include <string>

namespace priam
{
class Client;

class Prepared : public std::enable_shared_from_this<Prepared>
{
    /**
     * Client is given friend access to the private constructor so only Client's
     * can create Prepared objects correctly.
     */
    friend Client;

public:
    Prepared(const Prepared&) = delete;
    Prepared(Prepared&&)      = delete;
    auto operator=(const Prepared&) -> Prepared& = delete;
    auto operator=(Prepared &&) -> Prepared& = delete;

    /**
     * @return A statement that can have parameters bound an then Executed through the Client.
     */
    auto CreateStatement() const -> std::unique_ptr<Statement>;

private:
    /**
     * @param client The client that owns this prepared statement.
     * @param query The prepared statement query.
     * @throws std::runtime_error If the prepare setup fail to register or is malformed.
     */
    Prepared(Client& client, const std::string& query);

    CassPreparedPtr m_cass_prepared_ptr{nullptr}; ///< The underlying cassandra prepared object.
};

} // namespace priam
