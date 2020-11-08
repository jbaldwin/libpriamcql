#include <priam/priam.hpp>

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

static auto on_query_complete(priam::result result, std::atomic<uint64_t>& remaining) -> void
{
    std::cout << "Status code: " << priam::to_string(result.status()) << std::endl;
    std::cout << "Row count: " << result.row_count() << std::endl;
    std::cout << "Value count: " << result.column_count() << std::endl;

    for (const auto& row : result)
    {
        for (const auto& value : row)
        {
            std::cout << "DataType: " << priam::to_string(value.type()) << std::endl;
            if (value.is_null())
            {
                std::cout << "value: null" << std::endl;
                return;
            }

            switch (value.type())
            {
                case priam::data_type::custom:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case priam::data_type::ascii:
                    std::cout << "value: " << value.as_ascii().value_or("") << std::endl;
                    break;
                case priam::data_type::bigint:
                    std::cout << "value: " << value.as_big_int().value_or(0) << std::endl;
                    break;
                case priam::data_type::blob:
                {
                    auto opt = value.as_blob();
                    if (opt.has_value())
                    {
                        const auto& blob = opt.value();
                        std::cout << "value: " << std::string{reinterpret_cast<const char*>(blob.data()), blob.size()}
                                  << std::endl;
                    }
                    else
                    {
                        std::cout << "failed to convert to blob type" << std::endl;
                    }
                }
                break;
                case priam::data_type::boolean:
                    std::cout << "value: " << value.as_boolean().value_or(false) << std::endl;
                    break;
                case priam::data_type::counter:
                    std::cout << "value: " << value.as_counter().value_or(0) << std::endl;
                    break;
                case priam::data_type::decimal:
                {
                    auto opt = value.as_decimal();
                    if (opt.has_value())
                    {
                        const auto& decimal = opt.value();
                        const auto& blob    = decimal.varint();
                        const auto& scale   = decimal.scale();
                        std::cout << "value: " << std::string{reinterpret_cast<const char*>(blob.data()), blob.size()}
                                  << " scale: " << scale << std::endl;
                    }
                    else
                    {
                        std::cout << "failed to convert to decimal type." << std::endl;
                    }
                }
                break;
                case priam::data_type::double_t:
                    std::cout << "value: " << value.as_double().value_or(0.0) << std::endl;
                    break;
                case priam::data_type::float_t:
                    std::cout << "value: " << value.as_float().value_or(0.0f) << std::endl;
                    break;
                case priam::data_type::int_t:
                    std::cout << "value: " << value.as_int().value_or(0) << std::endl;
                    break;
                case priam::data_type::text:
                    std::cout << "value: " << value.as_text().value_or("") << std::endl;
                    break;
                case priam::data_type::timestamp:
                    std::cout << "string value: " << value.as_timestamp_date_formatted().value_or("") << std::endl;
                    std::cout << "time_t value: " << value.as_timestamp().value_or(0) << std::endl;
                    break;
                case priam::data_type::timeuuid:
                case priam::data_type::uuid:
                {
                    auto opt = value.as_uuid();
                    if (opt.has_value())
                    {
                        const auto& uuid = opt.value();
                        std::cout << "value: " << priam::to_string(uuid) << std::endl;
                    }
                }
                break;
                case priam::data_type::varchar:
                    std::cout << "value: " << value.as_varchar().value_or("") << std::endl;
                    break;
                case priam::data_type::varint:
                {
                    auto opt = value.as_varint();
                    if (opt.has_value())
                    {
                        const auto& blob = opt.value();
                        std::cout << "value: " << std::string{reinterpret_cast<const char*>(blob.data()), blob.size()}
                                  << std::endl;
                    }
                    else
                    {
                        std::cout << "failed to convert to varint" << std::endl;
                    }
                }
                break;
                case priam::data_type::inet:
                {
                    auto opt = value.as_inet();
                    if (opt.has_value())
                    {
                        std::cout << "value: " << opt.value() << std::endl;
                    }
                    else
                    {
                        std::cout << "failed to convert to inet" << std::endl;
                    }
                }
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case priam::data_type::date:
                    std::cout << "value: " << value.as_date().value_or(0) << std::endl;
                    break;
                case priam::data_type::time:
                    std::cout << "value: " << value.as_time().value_or(0) << std::endl;
                    break;
                case priam::data_type::smallint:
                    std::cout << "value: " << value.as_small_int().value_or(0) << std::endl;
                    break;
                case priam::data_type::tinyint:
                    std::cout << "value: " << value.as_tiny_int().value_or(0) << std::endl;
                    break;
                case priam::data_type::duration:
                {
                    auto opt = value.as_duration();
                    if (opt.has_value())
                    {
                        const auto& duration = opt.value();
                        std::cout << "months: " << duration.months() << " days: " << duration.days()
                                  << " nanos: " << duration.nanos() << std::endl;
                    }
                    else
                    {
                        std::cout << "failed to convert to duration" << std::endl;
                    }
                }
                case priam::data_type::list:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case priam::data_type::map:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case priam::data_type::set:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case priam::data_type::udt:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case priam::data_type::tuple:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case priam::data_type::unknown:
                    std::cout << "'unknown' cannot parse the value." << std::endl;
                    break;
            }
        }
    }

    // signal back to the main thread through the user_data the query has completed.
    remaining.fetch_sub(1, std::memory_order_relaxed);
}

int main(int argc, char* argv[])
{
    if (argc < 6)
    {
        std::cout << argv[0] << " <host> <port> <username> <password> <query>" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string host     = argv[1];
    uint16_t    port     = static_cast<uint16_t>(std::stoul(argv[2]));
    std::string username = argv[3];
    std::string password = argv[4];

    std::string raw_query = argv[5];

    auto cluster = priam::cluster::make_unique();
    cluster->add_host(std::move(host)).port(port).username_and_password(std::move(username), std::move(password));

    cluster->round_robin_load_balancing();
    cluster->token_aware_routing(true);
    cluster->heartbeat_interval(5s, 20s);

    /**
     * Using this library there should be one Cluster per set of clusters queries are issued against.
     */
    std::unique_ptr<priam::client> client_ptr{nullptr};

    /**
     * There will be as many Prepared objects are queries that you need to execute against the cluster.
     * These are contained in shared_ptr as the cassandra driver and the application share this information.
     */
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

    /**
     * Every query executed be a unique statement generated from a Prepared object to execute on the Client.
     * In this example only a single simple statement is executed.  Ownership of the statement is passed into
     * the Client when executed and cannot be 're-used'.  Generate another statement from the Prepared object
     * to issue another query.
     */
    auto statement = prepared_ptr->make_statement();

    // nothing to bind in this example yet

    /**
     * Execute the statement asynchronously with a 1 second timeout.  The Client driver will call
     * the on_query_complete callback with the result of the query (or timeout).  We'll pass a simple
     * int& through the user data to signal to the main thread the query has completed.
     */
    std::atomic<uint64_t> remaining{0};

    /**
     * Example using a std::bind function to add additional parameters to the callback.
     */
    using namespace std::placeholders;
    remaining.fetch_add(1, std::memory_order_relaxed);
    auto callback = std::bind(on_query_complete, _1, std::ref(remaining));
    client_ptr->execute_statement(statement, std::move(callback), 1s);

    /**
     * Example using a lambda function to add additional parameters to the callback through captures.
     */
    remaining.fetch_add(1, std::memory_order_relaxed);
    client_ptr->execute_statement(
        statement,
        [&remaining](priam::result result) {
            // do logic in lambda or call another function by std::move()ing the result.
            on_query_complete(std::move(result), remaining);
        },
        1s);

    // Wait for the query complete callback to finish, or timeout
    while (remaining > 0)
    {
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}
