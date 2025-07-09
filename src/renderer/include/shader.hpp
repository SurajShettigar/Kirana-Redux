// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_SHADER_HPP
#define KIRANA_RENDERER_SHADER_HPP

#include <vulkan/vulkan.hpp>
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

private:
    FilePath m_source_path{};

    vk::Device m_device{nullptr};
    vk::ShaderModule m_handle{nullptr};

    bool init(vk::Device device, const FilePath &source_path);
};
}

#endif //KIRANA_RENDERER_SHADER_HPP