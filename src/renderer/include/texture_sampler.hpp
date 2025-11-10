// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_TEXTURE_SAMPLER_HPP
#define KIRANA_RENDERER_TEXTURE_SAMPLER_HPP

#include <vulkan/vulkan.hpp>
#include "common.hpp"

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

class TextureSampler
{
    friend class Device;

public:
    TextureSampler() = default;
    ~TextureSampler() = default;

    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr;
    }

    [[nodiscard]] vk::Sampler getNativeHandle() const
    {
        return m_handle;
    }

    [[nodiscard]] SamplerData getData() const
    {
        return m_data;
    }

private:
    std::string m_name{};

    vk::Device m_device{nullptr};
    vk::Sampler m_handle{nullptr};

    SamplerData m_data{};

    bool init(vk::Device device, const std::string &name, const SamplerData &data = SamplerData{});
};
}

#endif //KIRANA_RENDERER_TEXTURE_SAMPLER_HPP