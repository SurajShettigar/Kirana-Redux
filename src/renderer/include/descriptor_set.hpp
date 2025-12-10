// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_DESCRIPTOR_SET_HPP
#define KIRANA_RENDERER_DESCRIPTOR_SET_HPP

#include <descriptor_layout.hpp>
#include "buffer.hpp"
#include "texture.hpp"
#include "texture_sampler.hpp"

namespace kirana::renderer
{
struct ShaderBindingResource
{
    uint32_t index = 0;

    union {
        const Buffer *buffer;
        const Texture *texture;
    };

    const TextureSampler *sampler;

    uint32_t count = 1;

    explicit ShaderBindingResource(const uint32_t index, const Buffer *buffer, const uint32_t count = 1)
        : index{index}, buffer{buffer}, sampler{nullptr}, count{count}
    {
    }

    explicit ShaderBindingResource(const uint32_t index, const Texture *texture,
                                   const TextureSampler *sampler = nullptr, const uint32_t count = 1)
        : index{index}, texture{texture}, sampler{sampler}, count{count}
    {
    }

    explicit ShaderBindingResource(const uint32_t index, const TextureSampler *sampler = nullptr,
                                   const uint32_t count = 1)
        : index{index}, texture{nullptr}, sampler{sampler}, count{count}
    {
    }
};

class DescriptorSet
{
    friend class DescriptorAllocator;

  public:
    DescriptorSet() = default;
    ~DescriptorSet() = default;

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr;
    }

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
    }

    [[nodiscard]] vk::DescriptorSet getNativeHandle() const
    {
        return m_handle;
    }

    void updateBindingResources(const DescriptorLayout &layout,
                                const std::vector<ShaderBindingResource> &resources) const;

  private:
    std::string m_name{};
    vk::Device m_device{nullptr};
    vk::DescriptorSet m_handle{nullptr};

    DescriptorSet(vk::Device device, vk::DescriptorSet handle, std::string name);
};
} // namespace kirana::renderer

#endif // KIRANA_RENDERER_DESCRIPTOR_SET_HPP