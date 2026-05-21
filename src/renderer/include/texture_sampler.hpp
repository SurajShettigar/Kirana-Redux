// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_TEXTURE_SAMPLER_HPP
#define KIRANA_RENDERER_TEXTURE_SAMPLER_HPP

#include "common.hpp"

#include <vulkan/vulkan.hpp>

#include <resource.hpp>
#include <handle.hpp>

namespace kirana::renderer
{

struct SamplerData
{
    SamplerFilterMode mag_filter{SamplerFilterMode::NEAREST};
    SamplerFilterMode min_filter{SamplerFilterMode::NEAREST};
    SamplerFilterMode mip_map_mode{SamplerFilterMode::NEAREST};
    SamplerWrapMode wrap_mode_u{SamplerWrapMode::REPEAT};
    SamplerWrapMode wrap_mode_v{SamplerWrapMode::REPEAT};
    uint8_t max_anisotropy{0u};

    bool operator==(const SamplerData &rhs) const
    {
        return mag_filter == rhs.mag_filter && min_filter == rhs.min_filter && mip_map_mode == rhs.mip_map_mode &&
               wrap_mode_u == rhs.wrap_mode_u && wrap_mode_v == rhs.wrap_mode_v && max_anisotropy == rhs.max_anisotropy;
    }
};

class TextureSampler;
using TextureSamplerHandle = core::Handle<TextureSampler>;

class TextureSampler final : public core::IResource
{
    friend class ResourceAllocator;

  public:
    TextureSampler() : IResource{"Texture_Sampler"}
    {
    }
    ~TextureSampler() override
    {
    }

    [[nodiscard]] bool isValid() const override
    {
        return IResource::isValid() && m_handle != nullptr;
    }

    [[nodiscard]] vk::Sampler getNativeHandle() const
    {
        return m_handle;
    }

    [[nodiscard]] SamplerData getData() const
    {
        return m_data;
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
    SamplerData m_data{};

    vk::Sampler m_handle{nullptr};

    explicit TextureSampler(const std::string &name, const SamplerData &data, vk::Sampler sampler)
        : IResource{name}, m_data{data}, m_handle{sampler}
    {
    }
};
} // namespace kirana::renderer

#endif // KIRANA_RENDERER_TEXTURE_SAMPLER_HPP