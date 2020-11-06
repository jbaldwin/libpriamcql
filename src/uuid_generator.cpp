#include "priam/uuid_generator.hpp"

namespace priam
{
auto to_string(uuid& uuid) -> std::string
{
    // The CASS_UUID_STRING_LENGTH includes the null terminator.
    std::string output;
    output.resize(CASS_UUID_STRING_LENGTH - 1);
    cass_uuid_string(uuid, output.data());
    return output;
}

uuid_generator::uuid_generator() : m_uuid_gen_ptr(cass_uuid_gen_new())
{
}

uuid_generator::uuid_generator(cass_uint64_t node) : m_uuid_gen_ptr(cass_uuid_gen_new_with_node(node))
{
}

auto uuid_generator::uuid_v1() -> uuid
{
    CassUuid uuid;
    cass_uuid_gen_time(m_uuid_gen_ptr.get(), &uuid);
    return uuid;
}

auto uuid_generator::uuid_v1(uint64_t epoch_timestamp) -> uuid
{
    CassUuid uuid;
    cass_uuid_gen_from_time(m_uuid_gen_ptr.get(), epoch_timestamp * 1'000, &uuid);
    return uuid;
}

auto uuid_generator::uuid_v4() -> uuid
{
    CassUuid uuid;
    cass_uuid_gen_random(m_uuid_gen_ptr.get(), &uuid);
    return uuid;
}

} // namespace priam
