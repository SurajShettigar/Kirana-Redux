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

    [[nodiscard]] ShaderStageFlags getStage() const
    {
        return m_stage;
    }

    [[nodiscard]] const std::string &getEntryPoint() const
    {
        return m_entry_point;
    }

    [[nodiscard]] vk::ShaderModule getNativeHandle() const
    {
        return m_handle;
    }

private:
    std::string m_name{};
    FilePath m_source_path{};
    ShaderStageFlags m_stage{};
    std::string m_entry_point{"main"};

    vk::Device m_device{nullptr};
    vk::ShaderModule m_handle{nullptr};

    bool init(vk::Device device, const std::string &name, const FilePath &source_path, ShaderStageFlags stage,
              const std::string &entry_point = "main");
};
}

#endif //KIRANA_RENDERER_SHADER_HPP