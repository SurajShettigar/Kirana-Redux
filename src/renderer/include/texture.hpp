// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_TEXTURE_HPP
#define KIRANA_RENDERER_TEXTURE_HPP

#include <vulkan/vulkan.hpp>
#include "common.hpp"

#include <memory_allocator.hpp>

namespace kirana::renderer
{

class Texture
{
    friend class Device;
    friend class CommandEncoder;
    friend class Swapchain;

public:
    Texture() = default;
    ~Texture() = default;

    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr && m_view != nullptr;
    }

    [[nodiscard]] const Size2D &getSize() const
    {
        return m_size;
    }

    [[nodiscard]] TextureFormat getFormat() const
    {
        return m_format;
    }

    [[nodiscard]] TextureLayout getLayout() const
    {
        return m_layout;
    }

    [[nodiscard]] vk::Image getNativeHandle() const
    {
        return m_handle;
    }

    [[nodiscard]] vk::ImageView getNativeViewHandle() const
    {
        return m_view;
    }

private:
    Size2D m_size{};
    TextureFormat m_format{};
    TextureUsageFlags m_usage{};
    TextureLayout m_layout{};

    vk::Device m_device{nullptr};
    vk::Image m_handle{nullptr};
    vk::ImageView m_view{nullptr};
    const MemoryAllocator *m_allocator{nullptr};

    AllocationID m_alloc_id{};

    bool init(vk::Device device, const Size2D &size, TextureFormat format,
              TextureUsageFlags usage = TextureUsageFlags::UNKNOWN,
              TextureLayout layout = TextureLayout::UNKNOWN, const MemoryAllocator *allocator = nullptr);

    explicit Texture(const vk::Device device, const vk::Image image, const vk::ImageView view, const Size2D &size,
                     const TextureFormat format, const TextureUsageFlags usage = TextureUsageFlags::UNKNOWN,
                     const TextureLayout layout = TextureLayout::UNKNOWN)
        : m_size{size}, m_format{format}, m_usage{usage}, m_layout{layout}, m_device{device}, m_handle{image},
          m_view{view}
    {
    }
};
}

#endif //KIRANA_RENDERER_TEXTURE_HPP