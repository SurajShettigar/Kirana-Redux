// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_BUFFER_HPP
#define KIRANA_RENDERER_BUFFER_HPP

#include "memory_allocator.hpp"
#include "common.hpp"

#include <resource.hpp>
#include <handle.hpp>

namespace kirana::renderer
{
class Buffer;
using BufferHandle = core::Handle<Buffer>;

class Buffer final : public core::IResource
{
  public:
    Buffer() : IResource{"Buffer"}
    {
    }
    explicit Buffer(const Device *device, const MemoryAllocator *allocator, const CommandEncoder *encoder,
                    const std::string &name, const uint64_t size, const void *data = nullptr,
                    const BufferUsageFlags usage = BufferUsageFlags::UNKNOWN)
        : IResource{name}, m_device{device}, m_allocator{allocator}, m_encoder{encoder}, m_usage{usage}, m_size{size},
          m_data{data}, m_is_dirty{m_data != nullptr}
    {
    }

    ~Buffer() override = default;

    [[nodiscard]] bool isValid() const override
    {
        return IResource::isValid() && m_handle != nullptr;
    }

    [[nodiscard]] uint64_t getSize() const
    {
        return m_size;
    }

    [[nodiscard]] vk::Buffer getNativeHandle() const
    {
        return m_handle;
    }

    [[nodiscard]] uint64_t getAddress() const;

    bool write(const CommandEncoder *encoder, const uint64_t size, const void *data)
    {
        m_encoder = encoder;
        m_size = size;
        m_data = data;
        m_is_dirty = true;
        return load();
    }

  protected:
    bool doLoad() override;
    void doUnload() override;

  private:
    const Device *m_device{nullptr};
    const MemoryAllocator *m_allocator{nullptr};
    const CommandEncoder *m_encoder{nullptr};

    BufferUsageFlags m_usage{};
    uint64_t m_size{};

    const void *m_data{nullptr};
    bool m_is_dirty{false};

    vk::Buffer m_handle{nullptr};
    AllocationID m_alloc_id{};

    bool doCreate();
    bool doWrite();
    void doDestroy();
};
} // namespace kirana::renderer

#endif // KIRANA_RENDERER_BUFFER_HPP