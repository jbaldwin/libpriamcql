#include <priam/CQL.hpp>

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

static auto again(
    priam::Result          result,
    std::atomic<bool>&     stop,
    priam::Client*         client,
    priam::Prepared*       prepared,
    std::atomic<uint64_t>& total,
    std::atomic<uint64_t>& success) -> void
{
    ++total;

    if (result.StatusCode() == CassError::CASS_OK)
    {
        ++success;
    }

    if (!stop)
    {
        client->ExecuteStatement(
            prepared->CreateStatement(), [&stop, client, prepared, &total, &success](priam::Result r) {
                again(std::move(r), stop, client, prepared, total, success);
            });
    }
}

int main(int argc, char* argv[])
{
    if (argc < 8)
    {
        std::cout << argv[0] << " <host> <port> <username> <password> <duration_seconds> <concurrent_requests> <query>"
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string          host     = argv[1];
    uint16_t             port     = static_cast<uint16_t>(std::stoul(argv[2]));
    std::string          username = argv[3];
    std::string          password = argv[4];
    std::chrono::seconds duration(static_cast<int64_t>(std::stol(argv[5])));
    uint64_t             concurrent_requests = static_cast<uint64_t>(std::stoul(argv[6]));

    std::string raw_query = argv[7];

    auto cluster = priam::Cluster::make();
    (*cluster).AddHost(std::move(host)).SetPort(port).SetUsernamePassword(std::move(username), std::move(password));

    cluster->SetRoundRobinLoadBalancing();
    cluster->SetTokenAwareRouting(true);
    cluster->SetHeartbeatInterval(5s, 20s);

    std::unique_ptr<priam::Client>   client_ptr{nullptr};
    std::shared_ptr<priam::Prepared> prepared_ptr{nullptr};

    try
    {
        /**
         * Setup everything in one go for this simple example, note that creating any of these
         * object types can fail for various reasons and will throw on a fatal error with an
         * underlying cause for the failure.
         */
        client_ptr   = std::make_unique<priam::Client>(std::move(cluster));
        prepared_ptr = client_ptr->CreatePrepared("name", raw_query);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    auto* client   = client_ptr.get();
    auto* prepared = prepared_ptr.get();

    std::atomic<bool>     stop{false};
    std::atomic<uint64_t> total{0};
    std::atomic<uint64_t> success{0};

    for (size_t i = 0; i < concurrent_requests; ++i)
    {
        client_ptr->ExecuteStatement(
            prepared_ptr->CreateStatement(),
            [&stop, client, prepared, &total, &success](priam::Result r) {
                again(std::move(r), stop, client, prepared, total, success);
            },
            1s);
    }

    // Wait for the query complete callback to finish, or timeout
    std::this_thread::sleep_for(duration);

    stop = true;

    std::cout << "Total: " << total << std::endl;
    std::cout << "Success: " << success << std::endl;
    std::cout << "Error: " << (total - success) << std::endl;

    std::cout << "QPS: " << (total / static_cast<uint64_t>(duration.count())) << std::endl;

    return 0;
}
