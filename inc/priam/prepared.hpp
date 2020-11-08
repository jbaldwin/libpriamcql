#pragma once

#include "priam/cpp_driver.hpp"
#include "priam/statement.hpp"

#include <memory>
#include <string_view>

namespace priam
{
class client;

class prepared : public std::enable_shared_from_this<prepared>
{
    /**
     * Client is given friend access to the private constructor so only Client's
     * can create prepared objects correctly.
     */
    friend client;

public:
    prepared(const prepared&) = delete;
    prepared(prepared&&)      = delete;
    auto operator=(const prepared&) -> prepared& = delete;
    auto operator=(prepared &&) -> prepared& = delete;

    /**
     * @return A statement that can have parameters bound an then Executed through the Client.
     */
    auto make_statement() const -> statement;

private:
    /**
     * @param client The client that owns this prepared statement.
     * @param query The prepared statement query.
     * @throws std::runtime_error If the prepare setup fail to register or is malformed.
     */
    prepared(client& client, std::string_view query);

    /// The underlying cassandra prepared object.
    cass_prepared_ptr m_cass_prepared_ptr{nullptr};
    /// The number of parameters to bind to this prepared statement.
    size_t m_parameter_count{0};
};

} // namespace priam
