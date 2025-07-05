// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "texture.hpp"

#include "helpers_vulkan.hpp"

namespace kirana::renderer
{
bool Texture::init(const vk::Device device, const Size2D &size, const TextureFormat format, const TextureLayout layout)
{
    m_device = device;
    m_size = size;
    m_format = format;
    m_layout = layout;
    const auto img_extent = getExtent3D(m_size);
    const auto img_format = renderer::getFormat(m_format);
    const auto img_layout = getImageLayout(m_layout);

    auto create_info = vk::ImageCreateInfo{vk::ImageCreateFlags{}, vk::ImageType::e2D, img_format, img_extent, 1, 1};
    create_info.setInitialLayout(img_layout);

    m_handle = m_device.createImage(create_info);

    const auto img_aspect = isDepthTextureFormat(m_format)
                                ? vk::ImageAspectFlagBits::eDepth
                                : vk::ImageAspectFlagBits::eColor;
    const auto view_create_info = vk::ImageViewCreateInfo{vk::ImageViewCreateFlags{}, m_handle, vk::ImageViewType::e2D,
                                                          img_format, vk::ComponentMapping{},
                                                          vk::ImageSubresourceRange{
                                                              img_aspect, 0, 1, 0, 1}};
    m_view = m_device.createImageView(view_create_info);

    return true;
}

void Texture::destroy()
{
    if (m_device)
    {
        if (m_handle)
        {
            m_device.destroyImage(m_handle);
            m_handle = nullptr;
        }
        if (m_view)
        {
            m_device.destroyImageView(m_view);
            m_view = nullptr;
        }
    }
}
}