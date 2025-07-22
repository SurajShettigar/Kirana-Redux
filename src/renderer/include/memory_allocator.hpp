// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_MEMORY_ALLOCATOR_HPP
#define KIRANA_RENDERER_MEMORY_ALLOCATOR_HPP

#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

namespace kirana::renderer
{
class AllocationID
{
    friend struct std::hash<AllocationID>;

public:
    AllocationID() = default;
    ~AllocationID() = default;

    explicit AllocationID(const uint32_t id) : m_id{id}
    {
    }

    explicit operator uint32_t() const
    {
        return m_id;
    }

    uint32_t operator()() const
    {
        return m_id;
    }

    bool operator==(const AllocationID &other) const
    {
        return m_id == other.m_id;
    }

    bool operator!=(const AllocationID &other) const
    {
        return m_id != other.m_id;
    }

    [[nodiscard]] bool isValid() const
    {
        return m_id < std::numeric_limits<uint32_t>::max();
    }

private:
    uint32_t m_id{std::numeric_limits<uint32_t>::max()};
};
}

template <>
struct std::hash<kirana::renderer::AllocationID>
{
    std::size_t operator()(const kirana::renderer::AllocationID &id) const noexcept
    {
        return hash<uint32_t>()(id.m_id);
    }
};

#include <unordered_map>

namespace kirana::renderer
{
class CommandEncoder;
class Fence;

class MemoryAllocator
{
    struct Allocation
    {
        VmaAllocation handle{};
        VmaAllocationInfo info{};
    };

    friend class Device;

public:
    MemoryAllocator() = default;
    ~MemoryAllocator() = default;

    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr;
    }

    AllocationID createImage(const vk::ImageCreateInfo &create_info, vk::Image *out_image) const;
    void destroyImage(AllocationID id, vk::Image image) const;

    AllocationID createBuffer(const CommandEncoder &encoder, const vk::BufferCreateInfo &create_info,
                              const uint8_t *data_buffer, vk::Buffer *out_buffer) const;
    void destroyBuffer(AllocationID id, vk::Buffer buffer) const;


    bool tryReleaseTemporaries(const Fence &fence) const;

private:
    VmaAllocator m_handle{nullptr};

    mutable uint32_t m_allocation_count{0};
    mutable std::unordered_map<AllocationID, Allocation> m_allocations{};
    mutable std::unordered_map<AllocationID, vk::Buffer> m_staging_buffers{};

    bool init(vk::Instance instance, vk::PhysicalDevice gpu, vk::Device device);
};
}

#endif //KIRANA_RENDERER_MEMORY_ALLOCATOR_HPP