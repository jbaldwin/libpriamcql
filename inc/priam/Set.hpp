#pragma once

#include "priam/cpp_driver.hpp"

namespace priam
{
class Value;

class Set
{
    friend Value; ///< For private constructor.
public:
    /**
     * Iterates over each Value in the Set and calls the 'value_callback' for each Value.
     * @tparam Functor The type signature for the functor 'value_callback'.
     * @param value_callback Functor Callback functor for each value in the list.  The parameter is 'const
     * priam::Value&' and each callback returns void.
     */
    template<typename Functor>
    auto ForEachValue(Functor&& value_callback) -> void;

private:
    const CassValue* m_cass_value{nullptr};

    explicit Set(const CassValue* cass_value);
};

} // namespace priam

#include "priam/Set.tcc"
