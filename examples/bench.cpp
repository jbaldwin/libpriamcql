#include <priam/priam.hpp>

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

static auto again(
    const priam::statement& stmt,
    priam::result           result,
    std::atomic<bool>&      stop,
    priam::client*          client,
    priam::prepared*        prepared,
    std::atomic<uint64_t>&  total,
    std::atomic<uint64_t>&  success) -> void
{
    total.fetch_add(1, std::memory_order_relaxed);

    if (result.status() == priam::status::ok)
    {
        success.fetch_add(1, std::memory_order_relaxed);
    }

    if (!stop.load(std::memory_order_relaxed))
    {
        client->execute_statement(stmt, [&stmt, &stop, client, prepared, &total, &success](priam::result r) {
            again(stmt, std::move(r), stop, client, prepared, total, success);
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

    auto cluster = priam::cluster::make_unique();
    cluster->add_host(std::move(host)).port(port).username_and_password(std::move(username), std::move(password));

    cluster->round_robin_load_balancing();
    cluster->token_aware_routing(true);
    cluster->heartbeat_interval(5s, 20s);

    std::unique_ptr<priam::client>   client_ptr{nullptr};
    std::shared_ptr<priam::prepared> prepared_ptr{nullptr};

    try
    {
        /**
         * Setup everything in one go for this simple example, note that creating any of these
         * object types can fail for various reasons and will throw on a fatal error with an
         * underlying cause for the failure.
         */
        client_ptr   = std::make_unique<priam::client>(std::move(cluster));
        prepared_ptr = client_ptr->prepared_register("name", raw_query);
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

    auto stmt = prepared_ptr->make_statement();

    for (size_t i = 0; i < concurrent_requests; ++i)
    {
        client_ptr->execute_statement(
            stmt,
            [&stmt, &stop, client, prepared, &total, &success](priam::result r) {
                again(stmt, std::move(r), stop, client, prepared, total, success);
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
