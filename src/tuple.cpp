#include "priam/tuple.hpp"

namespace priam
{
tuple::tuple(const CassValue* cass_value) : m_cass_value(cass_value)
{
}

} // namespace priam
