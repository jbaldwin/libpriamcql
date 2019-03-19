#pragma once

#include "priam/CppDriver.h"

namespace priam {

class UuidGenerator {
public:
    UuidGenerator();

    /**
     * @param node Creates a UuidGenerator with a uuid v1 using 'node' in the uuids.
     */
    explicit UuidGenerator(
        cass_uint64_t node);
    ~UuidGenerator() = default;
    UuidGenerator(const UuidGenerator&) = delete;
    UuidGenerator(UuidGenerator&&) noexcept = default;
    auto operator=(const UuidGenerator&) = delete;
    auto operator=(UuidGenerator&&) noexcept -> UuidGenerator& = default;

    /**
     * @return Generates a timestamp uuid v1.
     */
    auto GenTimeUuid() -> std::string;

    /**
     * @param epoch_nano_timestamp An epoch timestamp in nanos.
     * @return Generates a timestamp uuid v1 with the given timestamp.
     */
    auto GenTimeUuid(uint64_t epoch_nano_timestamp) -> std::string;

    /**
     * @return Generates a random uuid v4.
     */
    auto GenUuid() -> std::string;

private:
    CassUuidGenPtr m_uuid_gen_ptr { nullptr };
};

} // namespace priam
