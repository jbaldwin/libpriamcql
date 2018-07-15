#pragma once

#include "priam/CppDriver.h"

namespace priam
{

class Value;

class Tuple
{
    friend Value;
public:

    /**
     * Iterates over each Value in the Tuple and calls the 'value_callback' for each Value.
     * @tparam Functor The type signature for the functor 'value_callback'.
     * @param value_callback Functor Callback functor for each value in the Tuple.  The parameter is 'const priam::Value&' and
     *                       each callback returns void.
     */
    template<typename Functor>
    auto ForEachValue(
        Functor&& value_callback
    ) -> void;
private:
    const CassValue* m_cass_value{nullptr};

    explicit Tuple(
        const CassValue* cass_value
    );
};

} // namespace priam

#include "priam/Tuple.tcc"
