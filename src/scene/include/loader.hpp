// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_LOADER_HPP
#define KIRANA_SCENE_LOADER_HPP

#include "scene.hpp"

#include <array>

namespace kirana::scene
{
static const std::array<std::string, 2> EXTENSIONS_GLTF = {"gltf", "glb"};

enum class SceneFileFormat : uint8_t
{
    UNKNOWN = 0u,
    GLTF = 1u,
    COUNT = 2u,
};

struct SceneFileInfo
{
    std::string path{};
    std::string extension{};
    SceneFileFormat format{SceneFileFormat::UNKNOWN};

    [[nodiscard]] bool isValid() const
    {
        return !path.empty() && !extension.empty() && format != SceneFileFormat::UNKNOWN;
    }
};

/**
 * Loads scene file information from the given path.
 * @param path The path to the given scene file.
 * @return Scene file information.
 */
SceneFileInfo getSceneFileInfo(const std::string &path);


/**
 * Loads a scene from the given file info.
 * @param info The scene file information.
 * @param out_scene The scene object which will be populated with data.
 * @return true if successfully loaded scene data.
 */
bool loadScene(const SceneFileInfo &info, Scene *out_scene);

/**
 * Loads a scene and file information from the given file path.
 * @param path The path to the given scene file.
 * @param out_scene The scene object which will be populated with data. If the field is empty, only file information is
 * returned.
 * @return Scene file information.
 */
SceneFileInfo loadScene(const std::string &path, Scene *out_scene = nullptr);
} // namespace kirana::scene

#endif // KIRANA_SCENE_LOADER_HPP