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

bool Shader::init(const vk::Device device, const FilePath &source_path)
{
    m_device = device;
    if (!core::fileExists(source_path))
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Shader file does not exist: " + source_path.string());
        return false;
    }
    const auto file_size = core::getFileSize(source_path);
    if (file_size == 0)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Shader file is empty: " + source_path.string());
        return false;
    }
    std::vector<uint32_t> file_buffer(file_size / sizeof(uint32_t));
    core::readFile(source_path, true, reinterpret_cast<char *>(file_buffer.data()));

    const auto create_info = vk::ShaderModuleCreateInfo{vk::ShaderModuleCreateFlags{}, file_size, file_buffer.data()};
    m_handle = m_device.createShaderModule(create_info);
    return true;
}

}