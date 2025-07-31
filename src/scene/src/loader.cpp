// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "loader.hpp"

#include "gltf.hpp"

#include <logger.hpp>
#include <file_manager.hpp>

#include <algorithm>

namespace kirana::scene
{
inline SceneFileFormat getSceneFileFormat(const core::Filepath &filepath, std::string &out_extension)
{
    out_extension = core::getExtension(filepath);
    std::ranges::transform(out_extension, out_extension.begin(), ::tolower);

    if (!out_extension.empty())
    {
        const auto it = std::ranges::find_if(EXTENSIONS_GLTF, [&](const auto &e) {
            return e == out_extension;
        });
        if (it != EXTENSIONS_GLTF.end())
        {
            return SceneFileFormat::GLTF;
        }
    }

    {
        // Try to deduce the extension by reading the file header.
        const auto file_size = core::getFileSize(filepath);

        // Check for GLTF / GLB file format.
        // Refer: https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#binary-header
        if (file_size >= 12)
        {
            std::vector<uint32_t> header(3);
            core::readFileChunk(filepath, true, 0, 12, reinterpret_cast<char *>(header.data()));
            if (header[0] == 0x46546C67) // ASCII for gLTF
            {
                out_extension = "glb";
                return SceneFileFormat::GLTF;
            }
        }
    }

    return SceneFileFormat::UNKNOWN;
}

SceneFileInfo getSceneFileInfo(const std::string &path)
{
    SceneFileInfo info{path};

    const core::Filepath filepath{path};
    if (!core::fileExists(filepath))
    {
        core::Logger::error(LOG_CHANNEL_SCENE, "Invalid scene path: " + path);
        return info;
    }
    const size_t file_size = core::getFileSize(filepath);
    if (file_size == 0)
    {
        core::Logger::error(LOG_CHANNEL_SCENE, "Empty scene file: " + path);
        return info;
    }

    info.format = getSceneFileFormat(filepath, info.extension);
    return info;
}

bool loadScene(const SceneFileInfo &info, Scene *out_scene)
{
    if (!info.isValid())
    {
        return false;
    }

    std::string json_buffer{};
    GLTFDocument document{};
    if (const auto err = glz::read_file_json<glz::opts{.error_on_unknown_keys = false}>(
        document, info.path, json_buffer); err)
    {
        const std::string err_msg = glz::format_error(err, json_buffer);
        core::Logger::error(LOG_CHANNEL_SCENE, "Failed to parse GLTF json: " + err_msg);
        return false;
    }
    return true;
}

SceneFileInfo loadScene(const std::string &path, Scene *out_scene)
{
    SceneFileInfo info = getSceneFileInfo(path);
    if (out_scene == nullptr)
    {
        return info;
    }

    if (const auto status = loadScene(info, out_scene); !status)
    {
        core::Logger::error(LOG_CHANNEL_SCENE, "Failed to load scene: " + path);
    }
    return info;
}
}