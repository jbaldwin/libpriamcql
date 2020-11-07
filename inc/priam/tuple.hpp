#pragma once

#include "priam/cpp_driver.hpp"
#include "priam/value.hpp"

namespace priam
{
class tuple
{
    /// For private constructor.
    friend value;

public:
    /**
     * Iterates over each Value in the Tuple and calls the 'value_callback' for each Value.
     * @tparam functor_type The type signature for the functor 'value_callback'.
     * @param value_callback Functor Callback functor for each value in the Tuple.  The parameter is 'const
     * priam::Value&' and each callback returns void.
     */
    template<typename functor_type>
    auto for_each(functor_type&& value_callback) -> void
    {
        cass_iterator_ptr cass_iterator_ptr{cass_iterator_from_tuple(m_cass_value)};

        while (cass_iterator_next(cass_iterator_ptr.get()))
        {
            const CassValue* cass_value = cass_iterator_get_value(cass_iterator_ptr.get());
            if (cass_value != nullptr)
            {
                const priam::value value{cass_value};
                value_callback(value);
            }
        }
    }

private:
    const CassValue* m_cass_value{nullptr};

    explicit tuple(const CassValue* cass_value);
};

} // namespace priam
