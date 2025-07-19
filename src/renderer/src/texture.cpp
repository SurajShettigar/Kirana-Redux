// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "texture.hpp"

#include "helpers_vulkan.hpp"

namespace kirana::renderer
{
Texture::Texture(const vk::Device device, const std::string &name, const vk::Image image, const vk::ImageView view, const Size2D &size, const TextureFormat format, const TextureUsageFlags usage, const TextureLayout layout)
: m_name{name}, m_size{size}, m_format{format}, m_usage{usage}, m_layout{layout}, m_device{device}, m_handle{image},
  m_view{view}
{
    if (!m_name.empty())
    {
        auto handle = reinterpret_cast<uint64_t>(static_cast<VkImage>(m_handle));
        setDebugName(m_device, vk::ObjectType::eImage, handle, m_name);

        const auto view_name = m_name + "_View";
        handle = reinterpret_cast<uint64_t>(static_cast<VkImageView>(m_view));
        setDebugName(m_device, vk::ObjectType::eImageView, handle, view_name);
    }
}

bool Texture::init(const vk::Device device, const std::string &name, const Size2D &size, const TextureFormat format,
                   const TextureUsageFlags usage, const TextureLayout layout, const MemoryAllocator *allocator)
{
    m_device = device;
    m_name = name;
    m_allocator = allocator;

    m_size = size;
    m_format = format;
    m_usage = usage;
    m_layout = layout;
    const auto img_extent = getExtent3D(m_size);
    const auto img_format = renderer::getFormat(m_format);
    const auto img_usage = getImageUsageFlags(m_usage);
    const auto img_layout = getImageLayout(m_layout);

    auto create_info = vk::ImageCreateInfo{vk::ImageCreateFlags{}, vk::ImageType::e2D, img_format, img_extent, 1, 1,
                                           vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal, img_usage};
    create_info.setInitialLayout(img_layout);

    if (m_allocator)
    {
        m_alloc_id = m_allocator->createImage(create_info, &m_handle);
        if (!m_alloc_id.isValid())
        {
            return false;
        }
    }
    else
    {
        m_handle = m_device.createImage(create_info);
    }
    const auto img_aspect = isDepthTextureFormat(m_format)
                                ? vk::ImageAspectFlagBits::eDepth
                                : vk::ImageAspectFlagBits::eColor;
    const auto view_create_info = vk::ImageViewCreateInfo{vk::ImageViewCreateFlags{}, m_handle, vk::ImageViewType::e2D,
                                                          img_format, vk::ComponentMapping{},
                                                          vk::ImageSubresourceRange{
                                                              img_aspect, 0, 1, 0, 1}};
    m_view = m_device.createImageView(view_create_info);

    if (!m_name.empty())
    {
        auto handle = reinterpret_cast<uint64_t>(static_cast<VkImage>(m_handle));
        setDebugName(m_device, vk::ObjectType::eImage, handle, m_name);

        const auto view_name = m_name + "_View";
        handle = reinterpret_cast<uint64_t>(static_cast<VkImageView>(m_view));
        setDebugName(m_device, vk::ObjectType::eImageView, handle, view_name);
    }
    return true;
}

void Texture::destroy()
{
    if (m_device)
    {
        if (m_allocator && m_alloc_id.isValid())
        {
            m_allocator->destroyImage(m_alloc_id, m_handle);
            m_handle = nullptr;
            m_alloc_id = {};
        }
        else if (m_handle)
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