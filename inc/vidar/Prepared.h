#pragma once

#include "vidar/Client.h"
#include "vidar/Statement.h"
#include "vidar/Vidar.h"

#include <string>
#include <memory>

namespace vidar
{

class Prepared : public std::enable_shared_from_this<Prepared>
{
    friend Client;
public:
    virtual ~Prepared();

    auto CreateStatement() const -> std::unique_ptr<Statement>;

private:
    /**
     * @param client The client that owns this prepared statement.
     * @param query The prepared statement query.
     * @throws std::runtime_error If the prepare fails.
     */
    Prepared(Client& client, const std::string& query);

    const CassPrepared* m_prepared;
};

} // namespace vidar
