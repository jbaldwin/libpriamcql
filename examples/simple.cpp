#include "vidar/CQL.h"

#include <iostream>
#include <chrono>
#include <atomic>
#include <thread>
#include <cstdlib>

using namespace std::chrono_literals;

static auto on_query_complete(
    vidar::Result result,
    std::atomic<uint64_t>& remaining
) -> void
{
    std::cout << "Status code: " << vidar::to_string(result.GetStatusCode()) << std::endl;
    std::cout << "Row count: " << result.GetRowCount() << std::endl;
    std::cout << "Column count: " << result.GetColumnCount() << std::endl;

    /**
     * Iterate over each row returned by using a simple result iterator.
     */
    auto rows = result.GetIterator();
    for(size_t row_idx = 0; row_idx < result.GetRowCount(); ++row_idx)
    {
        /**
         * Grab the next row in the interator.  Note that without bounds checking
         * in the for loop this will throw a std::runtime_error if you go over
         * the maximum number of rows.
         */
        auto row = rows.GetNextRow();
        for(size_t col_idx = 0; col_idx < result.GetColumnCount(); ++col_idx)
        {
            /**
             * Grab each column in the row and prints its type + value.
             */
            auto column = row.GetColumn(col_idx);

            std::cout << "DataType: " << vidar::to_string(column.GetDataType()) << std::endl;
            switch(column.GetDataType())
            {
                case CASS_VALUE_TYPE_CUSTOM:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_ASCII:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_BIGINT:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_BLOB:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_BOOLEAN:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_COUNTER:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_DECIMAL:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_DOUBLE:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_FLOAT:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_INT:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_TEXT:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_TIMESTAMP:
                    std::cout << "string value: " << column.GetTimestampAsDateFormatted() << std::endl;
                    std::cout << "time_t value: " << column.GetTimestamp() << std::endl;
                    break;
                case CASS_VALUE_TYPE_UUID:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_VARCHAR:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_VARINT:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_TIMEUUID:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_INET:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_DATE:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_TIME:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_SMALL_INT:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_TINY_INT:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
                case CASS_VALUE_TYPE_DURATION:
                    std::cout << "type is currently unsupported" << std::endl;
                    break;
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
        }
    }

    // signal back to the main thread through the user_data the query has completed.
    --remaining;
}


int main(int argc, char* argv[])
{
    if(argc < 6)
    {
        std::cout << argv[0] << " <host> <port> <username> <password> <query>" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string host = argv[1];
    uint16_t port = static_cast<uint16_t>(std::stoul(argv[2]));
    std::string username = argv[3];
    std::string password = argv[4];

    std::string raw_query = argv[5];


    auto cluster = vidar::Cluster::make();
    (*cluster)
        .AddHost(std::move(host))
        .SetPort(port)
        .SetUsernamePassword(std::move(username), std::move(password));

    cluster->SetRoundRobinLoadBalancing();
    cluster->SetTokenAwareRouting(true);
    cluster->SetHeartbeatInterval(5s, 20s);

    /**
     * Using this library there should be one Cluster per set of clusters queries are issued against.
     */
    std::unique_ptr<vidar::Client> client_ptr{nullptr};

    /**
     * There will be as many Prepared objects are queries that you need to execute against the cluster.
     * These are contained in shared_ptr as the cassandra driver and the application share this information.
     */
    std::shared_ptr<vidar::Prepared> prepared_ptr{nullptr};

    /**
     * Every query executed be a unique Statement generated from a Prepared object to execute on the Client.
     * In this example only a single simple Statement is executed.  Ownership of the Statement is passed into
     * the Client when executed and cannot be 're-used'.  Generate another Statement from the Prepared object
     * to issue another query.
     */
    std::unique_ptr<vidar::Statement> statement_ptr1{nullptr};
    std::unique_ptr<vidar::Statement> statement_ptr2{nullptr};

    try
    {
        /**
         * Setup everything in one go for this simple example, note that creating any of these
         * object types can fail for various reasons and will throw on a fatal error with an
         * underlying cause for the failure.
         */
        client_ptr      = std::make_unique<vidar::Client>(std::move(cluster));
        prepared_ptr    = client_ptr->CreatePrepared(raw_query);
        statement_ptr1  = prepared_ptr->CreateStatement();
        statement_ptr2  = prepared_ptr->CreateStatement();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // nothing to bind in this example yet

    /**
     * Execute the Statement asynchronously with a 1 second timeout.  The Client driver will call
     * the on_query_complete callback with the Result of the query (or timeout).  We'll pass a simple
     * int& through the user data to signal to the main thread the query has completed.
     */
    std::atomic<uint64_t > remaining{0};

    /**
     * Example using a std::bind function to add additional parameters to the callback.
     */
    using namespace std::placeholders;
    ++remaining;
    auto callback = std::bind(on_query_complete, _1, std::ref(remaining));
    client_ptr->ExecuteStatement(std::move(statement_ptr1), std::move(callback), 1s);

    /**
     * Example using a lambda function to add additional parameters to the callback.
     */
    ++remaining;
    client_ptr->ExecuteStatement(
        std::move(statement_ptr2),
        [&remaining](vidar::Result result) {
            // do logic in lambda or call another function by std::move()ing the Result.
            on_query_complete(std::move(result), remaining);
        },
        1s
    );

    // Wait for the query complete callback to finish, or timeout
    while(remaining > 0)
    {
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}
