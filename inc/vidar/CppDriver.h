#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#include <cassandra.h>
#pragma clang diagnostic pop

#include <memory>

struct CassFutureDeleter
{
    auto operator()(CassFuture* cass_future) -> void
    {
        cass_future_free(cass_future);
    }
};

using CassFuturePtr = std::unique_ptr<CassFuture, CassFutureDeleter>;

struct CassResultDeleter
{
    auto operator()(const CassResult* cass_result) -> void
    {
        cass_result_free(cass_result);
    }
};

using CassResultPtr = std::unique_ptr<const CassResult, CassResultDeleter>;

struct CassIteratorDeleter
{
    auto operator()(CassIterator* cass_iterator) -> void
    {
        cass_iterator_free(cass_iterator);
    }
};

using CassIteratorPtr = std::unique_ptr<CassIterator, CassIteratorDeleter>;
