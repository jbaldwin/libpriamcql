#include "priam/Tuple.h"

namespace priam
{

Tuple::Tuple(
    const CassValue* cass_value
)
    : m_cass_value(cass_value)
{

}

} // namespace priam
