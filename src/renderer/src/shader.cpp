// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "shader.hpp"

#include "helpers_vulkan.hpp"

#include <logger.hpp>

namespace kirana::renderer
{
void Shader::destroy()
{
    if (m_device && m_handle)
    {
        m_device.destroyShaderModule(m_handle);
        m_handle = nullptr;
    }
}

bool Shader::init(const vk::Device device, const std::string &name, const FilePath &source_path,
                  const std::vector<ShaderStageFlags> &stages, const std::vector<std::string> &entry_points)
{
    if (stages.size() != entry_points.size())
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Number of entry points must equal number of stages.");
        return false;
    }
    if (entry_points.size() > 1)
    {
        const auto stage = stages[0];
        for (size_t i = 1; i < entry_points.size(); ++i)
        {
            if (stages[i] == stage)
            {
                core::Logger::error(LOG_CHANNEL_VULKAN,
                                    "Shader file cannot have multiple entry points for the same stage.");
                return false;
            }
        }
    }

    m_device = device;
    m_name = name;
    m_source_path = source_path;
    m_stages = stages;
    m_entry_points = entry_points;

    if (!core::fileExists(m_source_path))
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Shader file does not exist: " + m_source_path.string());
        return false;
    }
    const auto file_size = core::getFileSize(m_source_path);
    if (file_size == 0)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Shader file is empty: " + m_source_path.string());
        return false;
    }
    std::vector<uint32_t> file_buffer(file_size / sizeof(uint32_t));
    core::readFile(m_source_path, true, reinterpret_cast<char *>(file_buffer.data()));

    const auto create_info = vk::ShaderModuleCreateInfo{vk::ShaderModuleCreateFlags{}, file_size, file_buffer.data()};
    m_handle = m_device.createShaderModule(create_info);

    if (!m_name.empty())
    {
        const auto handle = reinterpret_cast<uint64_t>(static_cast<VkShaderModule>(m_handle));
        setDebugName(m_device, vk::ObjectType::eShaderModule, handle, m_name);
    }
    return true;
}

}