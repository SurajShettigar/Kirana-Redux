// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_IMAGE_HPP
#define KIRANA_SCENE_IMAGE_HPP

#include <string>
#include <vector>

#include <resource_manager.hpp>

namespace kirana::scene
{
enum class ImageChannelFormat: uint32_t
{
    UNKNOWN = 0u,
    UINT_8 = 1u,
    INT_8 = 2u,
    UINT_16 = 3u,
    INT_16 = 4u,
    UINT_32 = 5u,
    INT_32 = 6u,
    UINT_64 = 7u,
    INT_64 = 8u,
    HALF = 9u,
    FLOAT = 10u,
    DOUBLE = 11u,
};

class Image final : public core::IResource
{
public:
    static Image loadFromRawBuffer(const std::string &path, const std::vector<uint8_t> &buffer);

    Image() = default;
    /**
     * Loads image information.
     * @param path Path to the image file or data URI.
     * @param read_pixels If true, the pixel buffer is populated after initialization.
     */
    Image(std::string path, bool read_pixels = false);

    [[nodiscard]] bool isValid() const
    {
        return m_width > 0 && m_height > 0 && m_channels > 0 && m_format != ImageChannelFormat::UNKNOWN;
    }

    [[nodiscard]] const std::string &getPath() const
    {
        return m_path;
    }

    [[nodiscard]] uint32_t getWidth() const
    {
        return m_width;
    }

    [[nodiscard]] uint32_t getHeight() const
    {
        return m_height;
    }

    [[nodiscard]] uint32_t getNumChannels() const
    {
        return m_channels;
    }

    [[nodiscard]] ImageChannelFormat getChannelFormat() const
    {
        return m_format;
    }

    /// Returns the pixel buffer as-is. It does not attempt to read the image file if the buffer is empty.
    const std::vector<uint8_t> &getPixelBuffer() const
    {
        return m_pixels;
    }

    /// Returns the pixel-buffer. If the buffer was not read before, it will read from the file.
    /// @param convert_format If a valid format is specified, the pixel buffer will be converted to that format.
    const std::vector<uint8_t> &readPixelBuffer(ImageChannelFormat convert_format = ImageChannelFormat::UNKNOWN);

    void clearPixelBuffer()
    {
        if (!m_is_raw_buffer)
        {
            m_pixels.clear();
        }
    }

private:
    std::string m_path{};
    bool m_is_raw_buffer{false};

    uint32_t m_width{};
    uint32_t m_height{};
    uint32_t m_channels{};
    ImageChannelFormat m_format{};
    std::vector<uint8_t> m_pixels{};
};
}

#endif //KIRANA_SCENE_IMAGE_HPP