#pragma once

#include "priam/Statement.hpp"
#include "priam/cpp_driver.hpp"

#include <memory>
#include <string>

namespace priam
{
class client;

class Prepared : public std::enable_shared_from_this<Prepared>
{
    /**
     * Client is given friend access to the private constructor so only Client's
     * can create Prepared objects correctly.
     */
    friend client;

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
    Prepared(client& client, const std::string& query);

    cass_prepared_ptr m_cass_prepared_ptr{nullptr}; ///< The underlying cassandra prepared object.
};

} // namespace priam
