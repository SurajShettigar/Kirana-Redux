// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_IMAGE_HPP
#define KIRANA_CORE_IMAGE_HPP

#include <string>
#include <vector>

namespace kirana::core
{
class Image
{
public:
    static Image loadFromRawBuffer(const std::string &name, const std::vector<uint8_t> &buffer, std::vector<uint8_t> &out_pixels);

    Image() = default;
    ~Image() = default;

    /**
     * Loads image information. In the case of data uri, pixel buffer will be populated and `read_buffer` is ignored.
     * @param name Name of the image file.
     * @param path Path to the image file or data URI.
     */
    Image(std::string name, std::string path);

    [[nodiscard]] bool isValid() const
    {
        return m_width > 0 && m_height > 0 && m_channels > 0;
    }

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
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

    bool readPixels(std::vector<uint8_t> &out_buffer) const;
private:
    std::string m_name{};
    std::string m_path{};

    uint32_t m_width{};
    uint32_t m_height{};
    uint32_t m_channels{};
};
}

#endif //KIRANA_CORE_IMAGE_HPP