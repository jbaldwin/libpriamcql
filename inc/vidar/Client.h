#pragma once

#include "vidar/ConnectionInfo.h"
#include "vidar/OnCompleteCallback.h"
#include "vidar/Vidar.h"

#include <vector>
#include <memory>
#include <chrono>

namespace vidar
{

class Prepared;

using namespace std::chrono_literals;

class Client
{
    friend Prepared;
public:
    explicit Client(
        ConnectionInfo connection_info
        bool use_token_aware_routing
    );

    ~Client();

    auto CreatePrepared(
        const std::string& query
    ) -> std::shared_ptr<Prepared>;

    auto ExecuteStatement(
        std::unique_ptr<Statement> statement,
        void* data,
        OnCompleteCallback on_complete_callback,
        std::chrono::microseconds timeout = 0ms
    ) -> void;

private:
    ConnectionInfo m_connection_info;

    CassCluster* m_cluster{nullptr};
    CassSession* m_session{nullptr};

    std::vector<std::shared_ptr<Prepared>> m_prepared_statements;
};

} // namespace vidar
