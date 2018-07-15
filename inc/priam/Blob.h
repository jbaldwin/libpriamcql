#pragma once

#include <cstddef>

namespace priam
{

class Blob
{
public:
    Blob(
        const std::byte* bytes,
        size_t length
    )
        : m_bytes(bytes),
          m_length(length)
    {

    }

    /**
     * @return The raw bytes of the blob.
     */
    auto GetBytes() const -> const std::byte*
    {
        return m_bytes;
    }

    /**
     * @return The number of bytes in this blob.
     */
    auto GetLenth() const -> size_t
    {
        return m_length;
    }
private:
    const std::byte* m_bytes{nullptr};
    const size_t m_length{0};
};

} // namespace priam
