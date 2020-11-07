#pragma once

#include "priam/cpp_driver.hpp"

#include <string>

namespace priam
{
using uuid = CassUuid;

/**
 * Converts a CassUuid into a 36 byte string representation.
 * XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
 */
auto to_string(uuid& uuid) -> std::string;

class uuid_generator
{
public:
    uuid_generator();

    /**
     * @param node Creates a uuid_generator with a uuid v1 using 'node' in the uuids.
     */
    explicit uuid_generator(cass_uint64_t node);
    ~uuid_generator()                         = default;
    uuid_generator(const uuid_generator&)     = delete;
    uuid_generator(uuid_generator&&) noexcept = default;
    auto operator=(const uuid_generator&) -> uuid_generator& = delete;
    auto operator=(uuid_generator&&) noexcept -> uuid_generator& = default;

    /**
     * @return Generates a timestamp uuid v1.
     */
    auto uuid_v1() -> uuid;

    /**
     * @param epoch_timestamp An unix epoch timestamp.
     * @return Generates a timestamp uuid v1 with the given timestamp.
     */
    auto uuid_v1(uint64_t epoch_timestamp) -> uuid;

    /**
     * @return Generates a random uuid v4.
     */
    auto uuid_v4() -> uuid;

private:
    cass_uuid_gen_ptr m_uuid_gen_ptr{nullptr};
};

} // namespace priam
