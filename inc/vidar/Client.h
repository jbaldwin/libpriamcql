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
class Statement;
class Cluster;

using namespace std::chrono_literals;

class Client
{
    friend Prepared;
public:
    explicit Client(
        std::unique_ptr<Cluster> cluster_ptr,
        std::chrono::milliseconds connect_timeout = 30s
    );

    ~Client();

    auto CreatePrepared(
        const std::string& query
    ) -> std::shared_ptr<Prepared>;

    auto ExecuteStatement(
        std::unique_ptr<Statement> statement,
        OnCompleteCallback on_complete_callback,
        void* data = nullptr,
        std::chrono::microseconds timeout = 0ms,
        CassConsistency consistency = CassConsistency::CASS_CONSISTENCY_LOCAL_ONE
    ) -> void;

private:
    std::unique_ptr<Cluster> m_cluster_ptr{nullptr};
    CassSession* m_session{nullptr};

    std::vector<std::shared_ptr<Prepared>> m_prepared_statements;
};

} // namespace vidar
