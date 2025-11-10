// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "texture_sampler.hpp"

#include "helpers_vulkan.hpp"

namespace kirana::renderer
{
bool TextureSampler::init(const vk::Device device, const std::string &name, const SamplerData &data)
{
    m_device = device;
    m_name = name;

    m_data = data;

    auto create_info = vk::SamplerCreateInfo{vk::SamplerCreateFlags{}, getFilter(m_data.mag_filter),
                                             getFilter(m_data.min_filter),
                                             getSamplerMipmapMode(m_data.mip_map_mode),
                                             getSamplerAddressMode(m_data.wrap_mode_u),
                                             getSamplerAddressMode(m_data.wrap_mode_v),
                                             vk::SamplerAddressMode::eRepeat};
    if (m_data.max_anisotropy > 0)
    {
        create_info.setAnisotropyEnable(true);
        create_info.setMaxAnisotropy(m_data.max_anisotropy);
    }
    m_handle = m_device.createSampler(create_info);

    if (!m_name.empty())
    {
        const auto handle = reinterpret_cast<uint64_t>(static_cast<VkSampler>(m_handle));
        setDebugName(m_device, vk::ObjectType::eSampler, handle, m_name);
    }
    return true;
}

void TextureSampler::destroy()
{
    if (m_device && m_handle)
    {
        m_device.destroySampler(m_handle);
        m_handle = nullptr;
    }
}

}