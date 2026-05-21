// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_TEXTURE_HPP
#define KIRANA_RENDERER_TEXTURE_HPP

#include "common.hpp"

#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

#include <resource.hpp>
#include <handle.hpp>

namespace kirana::renderer
{
class Texture;
using TextureHandle = core::Handle<Texture>;

class Texture : public core::IResource
{
    friend class ResourceAllocator;
    friend class Swapchain;

  public:
    Texture() : IResource{"Texture"}
    {
    }
    ~Texture() override = default;

    [[nodiscard]] bool isValid() const override
    {
        return IResource::isValid() && m_handle != nullptr && m_view != nullptr;
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

    /// The function only updates the layout member variable and does not actually do anything. You need to use a
    /// CommandEncoder to actually transition the image layout.
    void setLayout(const TextureLayout layout) const
    {
        m_layout = layout;
    }

    [[nodiscard]] vk::Image getNativeHandle() const
    {
        return m_handle;
    }

    [[nodiscard]] vk::ImageView getNativeViewHandle() const
    {
        return m_view;
    }

    [[nodiscard]] VmaAllocation getNativeAllocation() const
    {
        return m_allocation;
    }

    [[nodiscard]] const VmaAllocationInfo &getNativeAllocationInfo() const
    {
        return m_alloc_info;
    }

  protected:
    bool doLoad() override
    {
        return true;
    }
    void doUnload() override
    {
    }

  private:
    Size2D m_size{};
    TextureFormat m_format{};
    TextureUsageFlags m_usage{};
    mutable TextureLayout m_layout{};

    vk::Image m_handle{nullptr};
    vk::ImageView m_view{nullptr};
    VmaAllocation m_allocation{nullptr};
    VmaAllocationInfo m_alloc_info{};

    explicit Texture(const std::string &name, const Size2D &size, const TextureFormat format,
                     const TextureUsageFlags usage, const TextureLayout layout, const vk::Image image,
                     const vk::ImageView view)
        : IResource{name}, m_size{size}, m_format{format}, m_usage{usage}, m_layout{layout}, m_handle{image},
          m_view{view}
    {
    }

    explicit Texture(const std::string &name, const Size2D &size, const TextureFormat format,
                     const TextureUsageFlags usage, const TextureLayout layout, vk::Image image, vk::ImageView view,
                     VmaAllocation allocation, const VmaAllocationInfo &alloc_info)
        : IResource{name}, m_size{size}, m_format{format}, m_usage{usage}, m_layout{layout}, m_handle{image},
          m_view{view}, m_allocation{allocation}, m_alloc_info{alloc_info}
    {
    }
};
} // namespace kirana::renderer

#endif // KIRANA_RENDERER_TEXTURE_HPP