#pragma once

#include "priam/Value.hpp"
#include "priam/cpp_driver.hpp"

namespace priam
{
class set
{
    /// For private constructor.
    friend Value;

public:
    /**
     * Iterates over each Value in the Set and calls the 'value_callback' for each Value.
     * @tparam functor_type The type signature for the functor 'value_callback'.
     * @param value_callback Functor Callback functor for each value in the list.  The parameter is 'const
     * priam::Value&' and each callback returns void.
     */
    template<typename functor_type>
    auto for_each(functor_type&& value_callback) -> void
    {
        cass_iterator_ptr cass_iterator_ptr{cass_iterator_from_collection(m_cass_value)};

        while (cass_iterator_next(cass_iterator_ptr.get()))
        {
            const CassValue* cass_value = cass_iterator_get_value(cass_iterator_ptr.get());
            if (cass_value != nullptr)
            {
                const priam::Value value{cass_value};
                value_callback(value);
            }
        }
    }

private:
    const CassValue* m_cass_value{nullptr};

    explicit set(const CassValue* cass_value);
};

} // namespace priam
