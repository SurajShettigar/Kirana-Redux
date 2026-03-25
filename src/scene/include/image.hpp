// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_IMAGE_HPP
#define KIRANA_SCENE_IMAGE_HPP

#include <string>
#include <vector>

#include <resource.hpp>

namespace kirana::scene
{
enum class ImageChannelFormat : uint32_t
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
    // static Image loadFromRawBuffer(const std::string &path, const std::vector<uint8_t> &buffer);

    Image() = default;

    /**
     * Loads image information.
     * @param path Path to the image file or data URI.
     */
    explicit Image(std::string path) : m_path{std::move(path)}
    {
    }

    /**
     * Reads the raw image buffer and saves it as a file in the given path with the given extension. This works only if
     * the raw image buffer corresponds to the image format provided in the file extension.
     * @param path Path where the image will be saved. Make sure filename contains a valid image extension.
     * @param raw_buffer Raw buffer (containing headers, pixel buffer, etc.) of the image.
     */
    explicit Image(std::string path, const std::vector<uint8_t> &raw_buffer);

    [[nodiscard]] bool isValid() const override
    {
        return IResource::isValid() && m_width > 0 && m_height > 0 && m_channels > 0 &&
               m_format != ImageChannelFormat::UNKNOWN;
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

    [[nodiscard]] bool isColorSpaceSRGB() const
    {
        return m_is_srgb;
    }

    void setColorSpace(const bool srgb)
    {
        m_is_srgb = srgb;
    }

    /// Writes the image content onto the given buffer.
    /// @param pixel_buffer The buffer to write the pixel values into.
    /// @param format If a valid format is specified, the pixel buffer will be converted to that format.
    /// @param num_channels If a value greater than 0 is provided, the pixel buffer will have the specified number of
    /// channels.
    bool readPixelBuffer(std::vector<uint8_t> &pixel_buffer, ImageChannelFormat format = ImageChannelFormat::UNKNOWN,
                         uint32_t num_channels = 0) const;

  protected:
    std::string m_path{};

    uint32_t m_width{};
    uint32_t m_height{};
    uint32_t m_channels{};
    ImageChannelFormat m_format{};
    bool m_is_srgb{false};

    bool doLoad() override;
    void doUnload() override
    {
        m_width = 0;
        m_height = 0;
        m_channels = 0;
        m_format = ImageChannelFormat::UNKNOWN;
        m_is_srgb = false;
    }
};
} // namespace kirana::scene

#endif // KIRANA_SCENE_IMAGE_HPP