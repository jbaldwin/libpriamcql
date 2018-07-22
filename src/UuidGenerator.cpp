#include "priam/UuidGenerator.h"

namespace priam
{

static auto to_string(CassUuid& uuid) -> std::string
{
    // use () instead of {}, doesn't always infer the correct ctor
    std::string output(CASS_UUID_STRING_LENGTH, '\0');
    cass_uuid_string(uuid, output.data());
    return output;
}

UuidGenerator::UuidGenerator()
    : m_uuid_gen_ptr(cass_uuid_gen_new())
{

}


UuidGenerator::UuidGenerator(
    cass_uint64_t node
)
    : m_uuid_gen_ptr(cass_uuid_gen_new_with_node(node))
{

}

auto UuidGenerator::GenTimeUuid() -> std::string
{
    CassUuid uuid;
    cass_uuid_gen_time(m_uuid_gen_ptr.get(), &uuid);
    return to_string(uuid);
}

auto UuidGenerator::GenTimeUuid(uint64_t epoch_nano_timestamp) -> std::string
{
    CassUuid uuid;
    cass_uuid_gen_from_time(m_uuid_gen_ptr.get(), epoch_nano_timestamp, &uuid);
    return to_string(uuid);
}

auto UuidGenerator::GenUuid() -> std::string
{
    CassUuid uuid;
    cass_uuid_gen_random(m_uuid_gen_ptr.get(), &uuid);
    return to_string(uuid);
}

} // namespace priam
