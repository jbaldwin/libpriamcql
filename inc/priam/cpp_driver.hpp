#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#include <cassandra.h>
#pragma clang diagnostic pop

#include <chrono>
#include <memory>

struct cass_session_deleter
{
    auto operator()(CassSession* cass_session) -> void
    {
        CassFuture* session_future = cass_session_close(cass_session);

        constexpr const std::chrono::milliseconds timeout{30};

        auto timed_out = !cass_future_wait_timed(
            session_future,
            static_cast<cass_duration_t>(std::chrono::duration_cast<std::chrono::microseconds>(timeout).count()));

        (void)timed_out; // don't want to throw but resources are going to get dropped here..

        cass_future_free(session_future);
        cass_session_free(cass_session);
    }
};

using cass_session_ptr = std::unique_ptr<CassSession, cass_session_deleter>;

struct cass_cluster_deleter
{
    auto operator()(CassCluster* cass_cluster) -> void { cass_cluster_free(cass_cluster); }
};

using cass_cluster_ptr = std::unique_ptr<CassCluster, cass_cluster_deleter>;

struct cass_future_deleter
{
    auto operator()(CassFuture* cass_future) -> void { cass_future_free(cass_future); }
};

using cass_future_ptr = std::unique_ptr<CassFuture, cass_future_deleter>;

struct cass_result_deleter
{
    auto operator()(const CassResult* cass_result) -> void { cass_result_free(cass_result); }
};

using cass_result_ptr = std::unique_ptr<const CassResult, cass_result_deleter>;

struct cass_prepared_deleter
{
    auto operator()(const CassPrepared* cass_prepared) -> void { cass_prepared_free(cass_prepared); }
};

using cass_prepared_ptr = std::unique_ptr<const CassPrepared, cass_prepared_deleter>;

struct cass_statement_deleter
{
    auto operator()(CassStatement* cass_statement) -> void { cass_statement_free(cass_statement); }
};

using cass_statement_ptr = std::unique_ptr<CassStatement, cass_statement_deleter>;

struct cass_iIterator_deleter
{
    auto operator()(CassIterator* cass_iterator) -> void { cass_iterator_free(cass_iterator); }
};

using cass_iterator_ptr = std::unique_ptr<CassIterator, cass_iIterator_deleter>;

struct cass_collection_deleter
{
    auto operator()(CassCollection* cass_collection) -> void { cass_collection_free(cass_collection); }
};

using cass_collection_ptr = std::unique_ptr<CassCollection, cass_collection_deleter>;

struct cass_uuid_gen_deleter
{
    auto operator()(CassUuidGen* cass_uuid_gen) -> void { cass_uuid_gen_free(cass_uuid_gen); }
};

using cass_uuid_gen_ptr = std::unique_ptr<CassUuidGen, cass_uuid_gen_deleter>;
