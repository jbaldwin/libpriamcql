#pragma once

#include "priam/cpp_driver.hpp"

#include <string>

namespace priam
{
template<typename type>
using ptr = type*;

using uuid = CassUuid;

/**
 * @param type Converts this CassValueType into a std::string representation.
 * @return The string representation.
 */
auto to_string(CassValueType type) -> const std::string&;

} // namespace priam
