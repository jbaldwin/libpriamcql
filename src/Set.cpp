#include "priam/Set.h"

namespace priam {

Set::Set(
    const CassValue* cass_value)
    : m_cass_value(cass_value)
{
}

} // namespace priam
