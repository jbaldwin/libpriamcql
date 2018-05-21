#include "vidar/CQL.h"

#include <iostream>
#include <chrono>
#include <atomic>
#include <thread>

using namespace std::chrono_literals;

static std::atomic<bool> g_done{false};

static auto on_query_complete(
    vidar::Result result,
    void* /*user_data*/
) -> void
{
    std::cout << "Row count: " << result.GetRowCount() << std::endl;
    std::cout << "Column count: " << result.GetColumnCount() << std::endl;

    auto rows = result.GetIterator();
    for(size_t row_idx = 0; row_idx < result.GetRowCount(); ++row_idx)
    {
        auto row = rows.GetNextRow();
        for(size_t col_idx = 0; col_idx < result.GetColumnCount(); ++col_idx)
        {
            auto column = row.GetColumn(col_idx);

            std::cout << "DataType: " << vidar::to_string(column.GetDataType()) << std::endl;
            switch(column.GetDataType())
            {

                case CASS_VALUE_TYPE_UNKNOWN:break;
                case CASS_VALUE_TYPE_CUSTOM:break;
                case CASS_VALUE_TYPE_ASCII:break;
                case CASS_VALUE_TYPE_BIGINT:break;
                case CASS_VALUE_TYPE_BLOB:break;
                case CASS_VALUE_TYPE_BOOLEAN:break;
                case CASS_VALUE_TYPE_COUNTER:break;
                case CASS_VALUE_TYPE_DECIMAL:break;
                case CASS_VALUE_TYPE_DOUBLE:break;
                case CASS_VALUE_TYPE_FLOAT:break;
                case CASS_VALUE_TYPE_INT:break;
                case CASS_VALUE_TYPE_TEXT:break;
                case CASS_VALUE_TYPE_TIMESTAMP:
                    std::cout << "StrValue: " << column.GetTimestampAsDateFormatted() << std::endl;
                    std::cout << "Value: " << column.GetTimestamp() << std::endl;
                    break;
                case CASS_VALUE_TYPE_UUID:break;
                case CASS_VALUE_TYPE_VARCHAR:break;
                case CASS_VALUE_TYPE_VARINT:break;
                case CASS_VALUE_TYPE_TIMEUUID:break;
                case CASS_VALUE_TYPE_INET:break;
                case CASS_VALUE_TYPE_DATE:break;
                case CASS_VALUE_TYPE_TIME:break;
                case CASS_VALUE_TYPE_SMALL_INT:break;
                case CASS_VALUE_TYPE_TINY_INT:break;
                case CASS_VALUE_TYPE_DURATION:break;
                case CASS_VALUE_TYPE_LIST:break;
                case CASS_VALUE_TYPE_MAP:break;
                case CASS_VALUE_TYPE_SET:break;
                case CASS_VALUE_TYPE_UDT:break;
                case CASS_VALUE_TYPE_TUPLE:break;
                case CASS_VALUE_TYPE_LAST_ENTRY:break;
            }
        }
    }

    g_done = true;
}


int main(int argc, char* argv[])
{
    if(argc < 6)
    {
        std::cout << argv[0] << " <host> <port> <username> <password> <query> <params...>" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string host = argv[1];
    uint16_t port = static_cast<uint16_t >(std::stoul(argv[2]));
    std::string username = argv[3];
    std::string password = argv[4];

    std::string raw_query = argv[5];

    vidar::ConnectionInfo connection_info;
    connection_info
        .AddHost(host)
        .SetPort(port)
        .SetUsername(username)
        .SetPassword(password);

    auto cluster = vidar::Cluster::make(std::move(connection_info));
    cluster->SetRoundRobinLoadBalancing();
    cluster->SetTokenAwareRouting(true);
    cluster->SetHeartbeatInterval(5s, 20s);

    std::unique_ptr<vidar::Client> client_ptr{nullptr};
    std::shared_ptr<vidar::Prepared> prepared_ptr{nullptr};
    std::unique_ptr<vidar::Statement> statement_ptr{nullptr};

    try
    {
        client_ptr = std::make_unique<vidar::Client>(std::move(cluster));
        prepared_ptr = client_ptr->CreatePrepared(std::move(raw_query));
        statement_ptr = prepared_ptr->CreateStatement();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // nothing to bind in this example yet

    client_ptr->ExecuteStatement(std::move(statement_ptr), on_query_complete, nullptr, 1s);

    // Wait for the query copmlete callback to finish, or timeout
    while(!g_done)
    {
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}
