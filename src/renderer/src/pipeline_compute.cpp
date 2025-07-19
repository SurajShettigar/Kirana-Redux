// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "pipeline_compute.hpp"

#include "helpers_vulkan.hpp"

#include <logger.hpp>

namespace kirana::renderer
{
bool PipelineCompute::init(const vk::Device device, const std::string &name, const PipelineLayout &layout, const Shader &shader)
{
    m_device = device;
    m_name = name;

    const auto stage_create_info = vk::PipelineShaderStageCreateInfo{vk::PipelineShaderStageCreateFlags{},
                                                                     static_cast<vk::ShaderStageFlagBits>(static_cast<
                                                                         uint32_t>(shader.getStage())),
                                                                     shader.getNativeHandle(),
                                                                     shader.getEntryPoint().c_str()};
    const auto create_info = vk::ComputePipelineCreateInfo{vk::PipelineCreateFlags{}, stage_create_info,
                                                           layout.getNativeHandle()};

    const auto result = m_device.createComputePipeline(nullptr, create_info);
    if (result.result != vk::Result::eSuccess)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to create compute pipeline: " + vk::to_string(result.result));
        return false;
    }
    m_handle = result.value;
    if (!m_name.empty())
    {
        const auto handle = reinterpret_cast<uint64_t>(static_cast<VkPipeline>(m_handle));
        setDebugName(m_device, vk::ObjectType::ePipeline, handle, m_name);
    }
    return true;
}

void PipelineCompute::destroy()
{
    if (m_device && m_handle)
    {
        m_device.destroyPipeline(m_handle);
        m_handle = nullptr;
    }
}


}