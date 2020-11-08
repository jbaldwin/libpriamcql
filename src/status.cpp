#include "priam/status.hpp"

namespace priam
{
auto to_string(status s) -> std::string
{
    return cass_error_desc(static_cast<CassError>(s));
}

} // namespace priam
