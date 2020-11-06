#include "priam/Tuple.hpp"

namespace priam
{
Tuple::Tuple(const CassValue* cass_value) : m_cass_value(cass_value)
{
}

} // namespace priam
