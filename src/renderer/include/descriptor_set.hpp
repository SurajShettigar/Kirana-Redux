// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_DESCRIPTOR_SET_HPP
#define KIRANA_RENDERER_DESCRIPTOR_SET_HPP

#include "descriptor_layout.hpp"
#include "buffer.hpp"
#include "texture.hpp"
#include "texture_sampler.hpp"

#include <variant>

namespace kirana::renderer
{
struct ShaderBindingResource
{
    uint32_t index = 0;
    std::variant<std::monostate, std::vector<BufferHandle>, std::vector<TextureHandle>> resources{};
    std::vector<TextureSamplerHandle> samplers{};

    explicit ShaderBindingResource(const uint32_t index, const BufferHandle buffer)
        : index{index}, resources{std::vector{buffer}}, samplers{}
    {
    }

    explicit ShaderBindingResource(const uint32_t index, std::vector<BufferHandle> buffers)
        : index{index}, resources{std::move(buffers)}, samplers{}
    {
    }

    explicit ShaderBindingResource(const uint32_t index, const TextureHandle texture,
                                   const TextureSamplerHandle sampler = {})
        : index{index}, resources{std::vector{texture}},
          samplers{sampler.isValid() ? std::vector{sampler} : std::vector<TextureSamplerHandle>{}}
    {
    }

    explicit ShaderBindingResource(const uint32_t index, std::vector<TextureHandle> textures,
                                   std::vector<TextureSamplerHandle> samplers = {})
        : index{index}, resources{std::move(textures)}, samplers{std::move(samplers)}
    {
    }

    explicit ShaderBindingResource(const uint32_t index, std::vector<TextureSamplerHandle> samplers)
        : index{index}, samplers{std::move(samplers)}
    {
    }

    bool isBuffer() const
    {
        return std::holds_alternative<std::vector<BufferHandle>>(resources);
    }
    bool isTexture() const
    {
        return std::holds_alternative<std::vector<TextureHandle>>(resources);
    }

    const std::vector<BufferHandle> &buffers() const
    {
        return std::get<std::vector<BufferHandle>>(resources);
    }
    const std::vector<TextureHandle> &textures() const
    {
        return std::get<std::vector<TextureHandle>>(resources);
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
    const Device *m_device{nullptr};

    std::string m_name{};
    vk::DescriptorSet m_handle{nullptr};

    DescriptorSet(const Device *device, std::string name, vk::DescriptorSet handle);
};
} // namespace kirana::renderer

#endif // KIRANA_RENDERER_DESCRIPTOR_SET_HPP