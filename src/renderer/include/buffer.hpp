// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_BUFFER_HPP
#define KIRANA_RENDERER_BUFFER_HPP

#include "common.hpp"

#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

#include <resource.hpp>
#include <handle.hpp>

namespace kirana::renderer
{
class Buffer;
using BufferHandle = core::Handle<Buffer>;

class Buffer final : public core::IResource
{
    friend class ResourceAllocator;

  public:
    Buffer() : IResource{"Buffer"}
    {
    }
    ~Buffer() override = default;


    [[nodiscard]] bool isValid() const override
    {
        return IResource::isValid() && m_handle != nullptr && m_allocation != nullptr;
    }

    [[nodiscard]] BufferUsageFlags getUsage() const
    {
        return m_usage;
    }

    [[nodiscard]] uint64_t getSize() const
    {
        return m_size;
    }

    [[nodiscard]] vk::Buffer getNativeHandle() const
    {
        return m_handle;
    }

    [[nodiscard]] VmaAllocation getNativeAllocation() const
    {
        return m_allocation;
    }

    [[nodiscard]] const VmaAllocationInfo &getNativeAllocationInfo() const
    {
        return m_alloc_info;
    }

    [[nodiscard]] uint64_t getAddress() const
    {
        return m_address;
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
    uint64_t m_size{};
    BufferUsageFlags m_usage{};

    vk::Buffer m_handle{nullptr};
    VmaAllocation m_allocation{nullptr};
    VmaAllocationInfo m_alloc_info{};
    uint64_t m_address{};

    explicit Buffer(const std::string &name, const uint64_t size, const BufferUsageFlags usage, vk::Buffer buffer,
                    VmaAllocation allocation, const VmaAllocationInfo &alloc_info, const uint64_t address)
        : IResource{name}, m_size{size}, m_usage{usage}, m_handle{buffer}, m_allocation{allocation},
          m_alloc_info{alloc_info}, m_address{address}
    {
    }
};
} // namespace kirana::renderer

#endif // KIRANA_RENDERER_BUFFER_HPP