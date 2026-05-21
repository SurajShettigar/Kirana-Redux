// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_RESOURCE_ALLOCATOR_HPP
#define KIRANA_RENDERER_RESOURCE_ALLOCATOR_HPP

#include "buffer.hpp"
#include "texture.hpp"
#include "texture_sampler.hpp"
#include "command_encoder.hpp"
#include "synchronization.hpp"

#include <resource_manager.hpp>
#include <no_copy.hpp>

namespace kirana::renderer
{
enum class ResourceMemoryType : uint8_t
{
    AUTO = 0,
    DEVICE = 1,
    HOST = 2,
};

enum class ResourceMemoryFlags : uint32_t
{
    UNKNOWN = 0x00000000u,
    /// Places the resource in its own memory block. Can be used for specialized textures resources like render targets.
    DEDICATED_MEMORY = 0x00000001u,
    /// The memory will be persistently mapped and can be accessed through a pointer. All resources on host memory can
    /// be mapped. But certain GPUs also support mapping of device local memory. So this flag can be used for both types
    /// (if supported).
    MAPPED = 0x00000004u,
    /// The memory will be mapped and can be written into sequentially. You may be able to read from it, but it will be
    /// very slow. Use HOST_ACCESS_RANDOM flag instead to be able to read GPU resources.
    HOST_ACCESS_SEQUENTIAL_WRITE = 0x00000400u,
    /// The memory will be mapped and can be written, read or accessed randomly.
    HOST_ACCESS_RANDOM = 0x00000800u,
    /// When used with HOST_ACCESS_SEQUENTIAL_WRITE or HOST_ACCESS_RANDOM, it chooses the best available memory type
    /// instead. It may be host visible or a device local memory. If it's device local, a staging buffer is created to
    /// transfer the data.
    HOST_ACCESS_ALLOW_TRANSFER_INSTEAD = 0x00001000u,
};


constexpr ResourceMemoryFlags operator|(const ResourceMemoryFlags lhs, const ResourceMemoryFlags rhs)
{
    return static_cast<ResourceMemoryFlags>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}

constexpr ResourceMemoryFlags &operator|=(ResourceMemoryFlags &lhs, const ResourceMemoryFlags rhs)
{
    return lhs = lhs | rhs;
}

constexpr ResourceMemoryFlags operator&(const ResourceMemoryFlags lhs, const ResourceMemoryFlags rhs)
{
    return static_cast<ResourceMemoryFlags>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
}

constexpr ResourceMemoryFlags &operator&=(ResourceMemoryFlags &lhs, const ResourceMemoryFlags rhs)
{
    return lhs = lhs & rhs;
}

constexpr ResourceMemoryFlags operator^(const ResourceMemoryFlags lhs, const ResourceMemoryFlags rhs)
{
    return static_cast<ResourceMemoryFlags>(static_cast<uint32_t>(lhs) ^ static_cast<uint32_t>(rhs));
}

constexpr ResourceMemoryFlags &operator^=(ResourceMemoryFlags &lhs, const ResourceMemoryFlags rhs)
{
    return lhs = lhs ^ rhs;
}

constexpr ResourceMemoryFlags operator~(const ResourceMemoryFlags flag)
{
    return static_cast<ResourceMemoryFlags>(~static_cast<uint32_t>(flag));
}

constexpr bool hasFlag(const ResourceMemoryFlags flags, const ResourceMemoryFlags req_flag)
{
    return (flags & req_flag) == req_flag;
}

class Device;
class ResourceAllocator : core::NoCopy
{
    friend class Device;

  public:
    ResourceAllocator() = default;
    ~ResourceAllocator() = default;

    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr;
    }

    /// Before any resource creation or writing data from host, this function needs to be called to initialize command
    /// buffer.
    void beginAllocation() const;
    /// After any resource creation or writing data from host, this function needs to be called to finalize command
    /// buffer and submit it to a queue.
    void endAllocation();

    BufferHandle createBuffer(const std::string &name, uint64_t size, BufferUsageFlags usage,
                              const void *data = nullptr, ResourceMemoryType memory_type = ResourceMemoryType::AUTO,
                              ResourceMemoryFlags memory_flags =
                                  ResourceMemoryFlags::HOST_ACCESS_SEQUENTIAL_WRITE | ResourceMemoryFlags::MAPPED |
                                  ResourceMemoryFlags::HOST_ACCESS_ALLOW_TRANSFER_INSTEAD);
    const Buffer *getBuffer(const BufferHandle handle) const
    {
        return m_buffers.get(handle);
    }
    bool writeBuffer(BufferHandle handle, uint64_t size, const void *data);
    void destroyBuffer(BufferHandle handle);

    TextureHandle createTexture(const std::string &name, const Size2D &size, TextureFormat format,
                                TextureUsageFlags usage, TextureLayout layout = TextureLayout::GENERAL,
                                const void *data = nullptr, ResourceMemoryType memory_type = ResourceMemoryType::DEVICE,
                                ResourceMemoryFlags memory_flags = ResourceMemoryFlags::UNKNOWN);
    const Texture *getTexture(const TextureHandle handle) const
    {
        return m_textures.get(handle);
    }
    bool writeImage(TextureHandle handle, const void *data);
    void destroyTexture(TextureHandle handle);

    TextureSamplerHandle createTextureSampler(const std::string &name, const SamplerData &data = {});
    const TextureSampler *getTextureSampler(const TextureSamplerHandle handle) const
    {
        return m_samplers.get(handle);
    }
    void destroyTextureSampler(TextureSamplerHandle handle);

  private:
    const Device *m_device{nullptr};
    VmaAllocator m_handle{nullptr};
    CommandEncoder m_encoder{};
    Fence m_fence{};

    std::vector<BufferHandle> m_staging_buffers{};
    core::ResourceManager<Buffer> m_buffers{};
    core::ResourceManager<Texture> m_textures{};
    core::ResourceManager<TextureSampler> m_samplers{};

    bool init(const Device *device);

    const Buffer *createStagingBuffer(const std::string &name, uint64_t size, const void *data);
};
} // namespace kirana::renderer

#endif // KIRANA_RENDERER_RESOURCE_ALLOCATOR_HPP