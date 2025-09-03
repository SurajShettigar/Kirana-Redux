// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_BUFFER_HPP
#define KIRANA_RENDERER_BUFFER_HPP

#include "memory_allocator.hpp"
#include "common.hpp"

namespace kirana::renderer
{
class Buffer
{
    friend class Device;

public:
    Buffer() = default;
    ~Buffer() = default;

    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr;
    }

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
    }

    [[nodiscard]] vk::Buffer getNativeHandle() const
    {
        return m_handle;
    }

    [[nodiscard]] uint64_t getSize() const
    {
        return m_size;
    }

    [[nodiscard]] uint64_t getAddress() const;

    bool update(const CommandEncoder &encoder, uint64_t size, const void *data);

private:
    std::string m_name{};
    uint64_t m_size{};
    BufferUsageFlags m_usage{};

    vk::Device m_device{nullptr};
    vk::Buffer m_handle{nullptr};
    const MemoryAllocator *m_allocator{nullptr};

    AllocationID m_alloc_id{};

    bool init(vk::Device device, const MemoryAllocator *allocator, const CommandEncoder &encoder,
              const std::string &name, uint64_t size, const void *data = nullptr,
              BufferUsageFlags usage = BufferUsageFlags::UNKNOWN);
};
}

#endif //KIRANA_RENDERER_BUFFER_HPP