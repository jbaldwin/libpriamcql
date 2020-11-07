#pragma once

#include <cstddef>

#include "priam/type.hpp"

namespace priam
{
class blob
{
public:
    blob(ptr<const std::byte> data, size_t size) : m_data(data), m_size(size) {}

    /**
     * @return The raw bytes of the blob.
     */
    auto data() const -> ptr<const std::byte> { return m_data; }

    /**
     * @return True if the blob has no bytes.
     */
    auto empty() const -> bool { return size() == 0; }

    /**
     * @return The number of bytes in this blob.
     */
    auto size() const -> size_t { return m_size; }

private:
    /// Pointer to the start of the blob's bytes.
    const ptr<const std::byte> m_data{nullptr};
    /// The number of bytes in this blob.
    const size_t m_size{0};
};

} // namespace priam
