#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#include <cassandra.h>
#pragma clang diagnostic pop

#include <chrono>
#include <memory>

struct CassSessionDeleter
{
    auto operator()(CassSession* cass_session) -> void
    {
        CassFuture* session_future = cass_session_close(cass_session);

        using namespace std::chrono_literals;
        std::chrono::milliseconds timeout   = 30s;
        auto                      timed_out = !cass_future_wait_timed(
            session_future,
            static_cast<cass_duration_t>(std::chrono::duration_cast<std::chrono::microseconds>(timeout).count()));

        (void)timed_out; // don't want to throw but resources are going to get dropped here..

        cass_future_free(session_future);
        cass_session_free(cass_session);
    }
};

using CassSessionPtr = std::unique_ptr<CassSession, CassSessionDeleter>;

struct CassClusterDeleter
{
    auto operator()(CassCluster* cass_cluster) -> void { cass_cluster_free(cass_cluster); }
};

using CassClusterPtr = std::unique_ptr<CassCluster, CassClusterDeleter>;

struct CassFutureDeleter
{
    auto operator()(CassFuture* cass_future) -> void { cass_future_free(cass_future); }
};

using CassFuturePtr = std::unique_ptr<CassFuture, CassFutureDeleter>;

struct CassResultDeleter
{
    auto operator()(const CassResult* cass_result) -> void { cass_result_free(cass_result); }
};

using CassResultPtr = std::unique_ptr<const CassResult, CassResultDeleter>;

struct CassPreparedDeleter
{
    auto operator()(const CassPrepared* cass_prepared) -> void { cass_prepared_free(cass_prepared); }
};

using CassPreparedPtr = std::unique_ptr<const CassPrepared, CassPreparedDeleter>;

struct CassStatementDeleter
{
    auto operator()(CassStatement* cass_statement) -> void { cass_statement_free(cass_statement); }
};

using CassStatementPtr = std::unique_ptr<CassStatement, CassStatementDeleter>;

struct CassIteratorDeleter
{
    auto operator()(CassIterator* cass_iterator) -> void { cass_iterator_free(cass_iterator); }
};

using CassIteratorPtr = std::unique_ptr<CassIterator, CassIteratorDeleter>;

struct CassCollectionDeleter
{
    auto operator()(CassCollection* cass_collection) -> void { cass_collection_free(cass_collection); }
};

using CassCollectionPtr = std::unique_ptr<CassCollection, CassCollectionDeleter>;

struct cass_uuid_gen_deleter
{
    auto operator()(CassUuidGen* cass_uuid_gen) -> void { cass_uuid_gen_free(cass_uuid_gen); }
};

using cass_uuid_gen_ptr = std::unique_ptr<CassUuidGen, cass_uuid_gen_deleter>;
