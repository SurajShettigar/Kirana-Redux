// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "loader.hpp"

#include "gltf.hpp"

#include <logger.hpp>

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
    if (const size_t file_size = core::getFileSize(filepath); file_size == 0)
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

    if (info.format == SceneFileFormat::GLTF)
    {
        const auto doc = GLTFLoader(info.path, true, true);
        return doc.isValid();
    }

    return false;
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