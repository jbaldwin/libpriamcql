#include <priam/priam.hpp>

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

static auto on_query_complete(priam::result result, std::atomic<uint64_t>& remaining) -> void
{
    std::cout << "Status code: " << priam::to_string(result.status_code()) << std::endl;
    std::cout << "Row count: " << result.row_count() << std::endl;
    std::cout << "Value count: " << result.column_count() << std::endl;

    /**
     * Iterate over each row returned by using a simple result iterator.
     */
    result.for_each([](const priam::row& row) -> void {
        row.for_each([](const priam::value& value) {
            std::cout << "DataType: " << priam::to_string(value.type()) << std::endl;
            if (value.is_null())
            {
                std::cout << "value: null" << std::endl;
                return;
            }

            switch (value.type())
            {
                case CASS_VALUE_TYPE_CUSTOM:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_ASCII:
                    std::cout << "value: " << value.as_ascii().value_or("") << std::endl;
                    break;
                case CASS_VALUE_TYPE_BIGINT:
                    std::cout << "value: " << value.as_big_int().value_or(0) << std::endl;
                    break;
                case CASS_VALUE_TYPE_BLOB:
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
                case CASS_VALUE_TYPE_BOOLEAN:
                    std::cout << "value: " << value.as_boolean().value_or(false) << std::endl;
                    break;
                case CASS_VALUE_TYPE_COUNTER:
                    std::cout << "value: " << value.as_counter().value_or(0) << std::endl;
                    break;
                case CASS_VALUE_TYPE_DECIMAL:
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
                case CASS_VALUE_TYPE_DOUBLE:
                    std::cout << "value: " << value.as_double().value_or(0.0) << std::endl;
                    break;
                case CASS_VALUE_TYPE_FLOAT:
                    std::cout << "value: " << value.as_float().value_or(0.0f) << std::endl;
                    break;
                case CASS_VALUE_TYPE_INT:
                    std::cout << "value: " << value.as_int().value_or(0) << std::endl;
                    break;
                case CASS_VALUE_TYPE_TEXT:
                    std::cout << "value: " << value.as_text().value_or("") << std::endl;
                    break;
                case CASS_VALUE_TYPE_TIMESTAMP:
                    std::cout << "string value: " << value.as_timestamp_date_formatted().value_or("") << std::endl;
                    std::cout << "time_t value: " << value.as_timestamp().value_or(0) << std::endl;
                    break;
                case CASS_VALUE_TYPE_TIMEUUID:
                case CASS_VALUE_TYPE_UUID:
                {
                    auto opt = value.as_uuid();
                    if (opt.has_value())
                    {
                        const auto& uuid = opt.value();
                        std::cout << "value: " << priam::to_string(uuid) << std::endl;
                    }
                }
                break;
                case CASS_VALUE_TYPE_VARCHAR:
                    std::cout << "value: " << value.as_varchar().value_or("") << std::endl;
                    break;
                case CASS_VALUE_TYPE_VARINT:
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
                case CASS_VALUE_TYPE_INET:
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
                case CASS_VALUE_TYPE_DATE:
                    std::cout << "value: " << value.as_date().value_or(0) << std::endl;
                    break;
                case CASS_VALUE_TYPE_TIME:
                    std::cout << "value: " << value.as_time().value_or(0) << std::endl;
                    break;
                case CASS_VALUE_TYPE_SMALL_INT:
                    std::cout << "value: " << value.as_small_int().value_or(0) << std::endl;
                    break;
                case CASS_VALUE_TYPE_TINY_INT:
                    std::cout << "value: " << value.as_tiny_int().value_or(0) << std::endl;
                    break;
                case CASS_VALUE_TYPE_DURATION:
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
                case CASS_VALUE_TYPE_LIST:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_MAP:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_SET:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_UDT:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_TUPLE:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_LAST_ENTRY:
                case CASS_VALUE_TYPE_UNKNOWN:
                    std::cout << "UNKNOWN/LAST_ENTRY cannot parse the value." << std::endl;
                    break;
            }
        });
    });

    // signal back to the main thread through the user_data the query has completed.
    --remaining;
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

    /**
     * Every query executed be a unique statement generated from a Prepared object to execute on the Client.
     * In this example only a single simple statement is executed.  Ownership of the statement is passed into
     * the Client when executed and cannot be 're-used'.  Generate another statement from the Prepared object
     * to issue another query.
     */
    std::unique_ptr<priam::statement> statement_ptr1{nullptr};
    std::unique_ptr<priam::statement> statement_ptr2{nullptr};

    try
    {
        /**
         * Setup everything in one go for this simple example, note that creating any of these
         * object types can fail for various reasons and will throw on a fatal error with an
         * underlying cause for the failure.
         */
        client_ptr     = std::make_unique<priam::client>(std::move(cluster));
        prepared_ptr   = client_ptr->prepared_register("name", raw_query);
        statement_ptr1 = prepared_ptr->make_statement();
        statement_ptr2 = prepared_ptr->make_statement();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

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
    ++remaining;
    auto callback = std::bind(on_query_complete, _1, std::ref(remaining));
    client_ptr->execute_statement(std::move(statement_ptr1), std::move(callback), 1s);

    /**
     * Example using a lambda function to add additional parameters to the callback through captures.
     */
    ++remaining;
    client_ptr->execute_statement(
        std::move(statement_ptr2),
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
