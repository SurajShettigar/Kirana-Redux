// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_TEXTURE_HPP
#define KIRANA_RENDERER_TEXTURE_HPP

#include "device.hpp"

namespace kirana::renderer
{
class Texture
{
    friend class Swapchain;

public:
    Texture() = default;
    ~Texture() = default;

    bool init(const Device &device, const Size2D &size, TextureFormat format);
    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr && m_view != nullptr;
    }

private:
    vk::Device m_device{nullptr};

    Size2D m_size{};
    TextureFormat m_format{};

    vk::Image m_handle{nullptr};
    vk::ImageView m_view{nullptr};

    explicit Texture(const vk::Device device, const vk::Image image, const vk::ImageView view, const Size2D &size,
                     const TextureFormat format)
        : m_device{device}, m_size{size}, m_format{format}, m_handle{image}, m_view{view}
    {
    }
};
}

#endif //KIRANA_RENDERER_TEXTURE_HPP