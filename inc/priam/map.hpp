#pragma once

#include "priam/Value.hpp"
#include "priam/cpp_driver.hpp"

namespace priam
{
class map
{
    friend Value; ///< For private constructor.
public:
    /**
     * Iterates over each Value in the List and calls the 'key_value_callback' for each Key Value pair.
     * @tparam functor_type The type signature for the functor 'key_value_callback'.
     * @param key_value_callback Functor Callback functor for each key value in the map.
     *                           The parameters are two 'const priam::Value&', one for the key and one for the value.
     *                           The functor callback returns null.
     */
    template<typename functor_type>
    auto for_each(functor_type&& key_value_callback) -> void
    {
        cass_iterator_ptr cass_iterator_ptr{cass_iterator_from_map(m_cass_value)};

        while (cass_iterator_next(cass_iterator_ptr.get()))
        {
            const CassValue* cass_value_key   = cass_iterator_get_map_key(cass_iterator_ptr.get());
            const CassValue* cass_value_value = cass_iterator_get_map_value(cass_iterator_ptr.get());
            if (cass_value_key != nullptr && cass_value_value != nullptr)
            {
                const priam::Value key{cass_value_key};
                const priam::Value value{cass_value_value};
                key_value_callback(key, value);
            }
        }
    }

private:
    // Non-owning cass value borrow.
    const CassValue* m_cass_value{nullptr};

    explicit map(const CassValue* cass_value);
};

} // namespace priam
