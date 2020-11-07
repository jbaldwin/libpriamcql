#include "priam/Set.hpp"
#include "priam/Value.hpp"

namespace priam {

template <typename Functor>
auto Set::ForEachValue(
    Functor&& value_callback) -> void
{
    cass_iterator_ptr cass_iterator_ptr { cass_iterator_from_collection(m_cass_value) };

    while (cass_iterator_next(cass_iterator_ptr.get())) {
        const CassValue* cass_value = cass_iterator_get_value(cass_iterator_ptr.get());
        if (cass_value != nullptr) {
            const priam::Value value { cass_value };
            value_callback(value);
        }
    }
}

} // namespace priam
