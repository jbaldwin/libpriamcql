#pragma once

#include "priam/Row.h"

namespace priam {

template <typename Functor>
auto Row::ForEachColumn(Functor&& value_callback) const -> void
{
    CassIteratorPtr cass_iterator_ptr(cass_iterator_from_row(m_cass_row));

    while (cass_iterator_next(cass_iterator_ptr.get())) {
        const CassValue* cass_value = cass_iterator_get_column(cass_iterator_ptr.get());
        const priam::Value value(cass_value);
        value_callback(value);
    }
}

} // namespace priam
