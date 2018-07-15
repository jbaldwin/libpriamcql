#pragma once

#include "priam/Result.h"

namespace priam
{

template<typename Functor>
auto Result::ForEachRow(
    Functor&& row_callback
) const -> void
{
    CassIteratorPtr cass_iterator_ptr{cass_iterator_from_result(m_cass_result_ptr.get())};

    while(cass_iterator_next(cass_iterator_ptr.get()))
    {
        const CassRow* cass_row = cass_iterator_get_row(cass_iterator_ptr.get());
        if(cass_row != nullptr)
        {
            const priam::Row row{cass_row};
            row_callback(row);
        }
    }
}

} // namespace priam
