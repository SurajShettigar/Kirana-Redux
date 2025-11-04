// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_SHADER_HPP
#define KIRANA_RENDERER_SHADER_HPP

#include <vulkan/vulkan.hpp>

#include "common.hpp"

#include <file_manager.hpp>

namespace kirana::renderer
{
class Shader
{
    using FilePath = core::Filepath;
    friend class Device;

public:
    Shader() = default;
    ~Shader() = default;

    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr;
    }

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
    }

    [[nodiscard]] const FilePath &getSourcePath() const
    {
        return m_source_path;
    }

    [[nodiscard]] const std::vector<ShaderStageFlags> &getStages() const
    {
        return m_stages;
    }

    [[nodiscard]] const std::vector<std::string> &getEntryPoints() const
    {
        return m_entry_points;
    }

    [[nodiscard]] vk::ShaderModule getNativeHandle() const
    {
        return m_handle;
    }

private:
    std::string m_name{};
    FilePath m_source_path{};
    std::vector<ShaderStageFlags> m_stages{};
    std::vector<std::string> m_entry_points{};

    vk::Device m_device{nullptr};
    vk::ShaderModule m_handle{nullptr};

    bool init(vk::Device device, const std::string &name, const FilePath &source_path,
              const std::vector<ShaderStageFlags> &stages = {ShaderStageFlags::COMPUTE},
              const std::vector<std::string> &entry_points = {"main"});
};
}

#endif //KIRANA_RENDERER_SHADER_HPP