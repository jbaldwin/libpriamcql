#include "priam/Map.h"

namespace priam
{

Map::Map(
    const CassValue* cass_value
)
    : m_cass_value(cass_value)
{

}

} // namespace priam