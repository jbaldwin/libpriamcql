#pragma once

#include <cstddef>

#include "priam/Type.hpp"

namespace priam
{
class Blob
{
public:
    Blob(const ptr<const std::byte> bytes, size_t length) : m_bytes(bytes), m_length(length) {}

    /**
     * @return The raw bytes of the blob.
     */
    auto Bytes() const -> ptr<const std::byte> { return m_bytes; }

    /**
     * @return The number of bytes in this blob.
     */
    auto Length() const -> size_t { return m_length; }

private:
    const ptr<const std::byte> m_bytes{nullptr};
    const size_t               m_length{0};
};

} // namespace priam
