#pragma once

#include "priam/CppDriver.h"

namespace priam
{

class Value;

class Map
{
    friend Value; ///< For private constructor.
public:

    /**
     * Iterates over each Value in the List and calls the 'key_value_callback' for each Key Value pair.
     * @tparam Functor The type signature for the functor 'key_value_callback'.
     * @param key_value_callback Functor Callback functor for each key value in the map.
     *                           The parameters are two 'const priam::Value&', one for the key and one for the value.
     *                           The functor callback returns null.
     */
    template<typename Functor>
    auto ForEachKeyValue(
        Functor&& key_value_callback
    ) -> void;
private:
    const CassValue* m_cass_value{nullptr};

    explicit Map(
        const CassValue* cass_value
    );
};

} // namespace priam

#include "priam/Map.tcc"
