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
    return SceneFileFormat::UNKNOWN;
}

inline std::unordered_map<uint32_t, std::vector<MeshHandle>> loadGLTFMeshes(GLTFLoader &loader, Scene *out_scene)
{
    const auto &doc = loader.getDocument();

    std::unordered_map<uint32_t, std::vector<MeshHandle>> meshes{};
    for (uint32_t m_index = 0; m_index < doc.meshes.size(); ++m_index)
    {
        const auto &mesh = doc.meshes[m_index];
        std::vector<MeshHandle> mesh_handles{};
        mesh_handles.reserve(mesh.primitives.size());

        std::string mesh_name{mesh.name.value_or("")};
        // TODO: Add support for meshes with morph targets.
        for (const auto &prim : mesh.primitives)
        {
            if (prim.mode != GLTFPrimitiveMode::TRIANGLES)
            {
                // TODO: Add support for non-triangle meshes.
                core::Logger::warn(LOG_CHANNEL_SCENE,
                                   "Unsupported primitive mode: " + std::to_string(static_cast<int>(prim.mode)) +
                                   " for mesh: " + std::to_string(m_index));
                continue;
            }
            IndexBuffer index_buffer{};
            if (prim.indices)
            {
                const auto &accessor = doc.accessors.at(prim.indices.value());
                if (accessor.component_type == GLTFComponentType::UINT_8)
                {
                    index_buffer.indices_8.resize(accessor.count);
                    loader.loadAccessorData(accessor, index_buffer.indices_8.data());
                }
                else if (accessor.component_type == GLTFComponentType::UINT_16)
                {
                    index_buffer.indices_16.resize(accessor.count);
                    loader.loadAccessorData(accessor, reinterpret_cast<uint8_t *>(index_buffer.indices_16.data()));
                }
                else if (accessor.component_type == GLTFComponentType::UINT_32)
                {
                    index_buffer.indices.resize(accessor.count);
                    loader.loadAccessorData(accessor, reinterpret_cast<uint8_t *>(index_buffer.indices.data()));
                }
                else
                {
                    core::Logger::error(LOG_CHANNEL_SCENE,
                                        "Unsupported index buffer component type: " + std::to_string(
                                            static_cast<int>(accessor.component_type)));
                    continue;
                }
            }
            VertexBuffer vertex_buffer{};
            // Access Positions
            if (const auto &pos_accessors = prim.getAttributeAccessor(GLTFPrimitiveAttribute::POSITION);
                !pos_accessors.empty())
            {

                const auto &pos_accessor = doc.accessors.at(pos_accessors.front());
                if (pos_accessor.type != GLTFAccessorType::VEC_3)
                {
                    core::Logger::error(LOG_CHANNEL_SCENE,
                                        "Unsupported position attribute accessor type: " + std::to_string(
                                            static_cast<int>(pos_accessor.type)));
                    continue;
                }
                std::vector<std::array<Float, 3>> positions{};
                positions.resize(pos_accessor.count);
                loader.loadAccessorData(pos_accessor, reinterpret_cast<uint8_t *>(positions.data()));
                vertex_buffer.setPositions(positions);
            }
            else
            {
                core::Logger::error(LOG_CHANNEL_SCENE,
                                    "Mesh: " + std::to_string(m_index) + " does not have a position attribute");
                continue;
            }
            // Access Normals
            if (const auto &normal_accessors = prim.getAttributeAccessor(GLTFPrimitiveAttribute::NORMAL);
                !normal_accessors.empty())
            {
                const auto &normal_accessor = doc.accessors.at(normal_accessors.front());
                if (normal_accessor.type != GLTFAccessorType::VEC_3)
                {
                    core::Logger::error(LOG_CHANNEL_SCENE,
                                        "Unsupported normal attribute accessor type: " + std::to_string(
                                            static_cast<int>(normal_accessor.type)));
                    continue;
                }
                std::vector<std::array<Float, 3>> normals{};
                normals.resize(normal_accessor.count);
                loader.loadAccessorData(normal_accessor, reinterpret_cast<uint8_t *>(normals.data()));
                vertex_buffer.setNormals(normals);
            }
            // Access UVs
            // TODO: Add support for N-number of texture coordinates for mesh.
            if (const auto &uv_accessors = prim.getAttributeAccessor(GLTFPrimitiveAttribute::TEX_COORD);
                !uv_accessors.empty())
            {
                const auto &uv_accessor = doc.accessors.at(uv_accessors.front());
                if (uv_accessor.type != GLTFAccessorType::VEC_2)
                {
                    core::Logger::error(LOG_CHANNEL_SCENE,
                                        "Unsupported texture coordinate attribute accessor type: " + std::to_string(
                                            static_cast<int>(uv_accessor.type)));
                    continue;
                }
                std::vector<std::array<Float, 2>> uvs{};
                uvs.resize(uv_accessor.count);
                loader.loadAccessorData(uv_accessor, reinterpret_cast<uint8_t *>(uvs.data()));
                vertex_buffer.setUVs(uvs);
            }
            // Access Colors
            // TODO: Add support for N-number of vertex colors for mesh.
            if (const auto &color_accessors = prim.getAttributeAccessor(GLTFPrimitiveAttribute::COLOR);
                !color_accessors.empty())
            {
                const auto &color_accessor = doc.accessors.at(color_accessors.front());
                if (color_accessor.type != GLTFAccessorType::VEC_3 && color_accessor.type != GLTFAccessorType::VEC_4)
                {
                    core::Logger::error(LOG_CHANNEL_SCENE,
                                        "Unsupported color attribute accessor type: " + std::to_string(
                                            static_cast<int>(color_accessor.type)));
                    continue;
                }
                if (color_accessor.type == GLTFAccessorType::VEC_3)
                {
                    std::vector<std::array<Float, 3>> colors{};
                    colors.resize(color_accessor.count);
                    loader.loadAccessorData(color_accessor, reinterpret_cast<uint8_t *>(colors.data()));
                    vertex_buffer.setColors(colors);
                }
                else
                {
                    std::vector<std::array<Float, 4>> colors{};
                    colors.resize(color_accessor.count);
                    loader.loadAccessorData(color_accessor, reinterpret_cast<uint8_t *>(colors.data()));
                    vertex_buffer.setColors(colors);
                }
            }
            mesh_handles.emplace_back(out_scene->addMesh(mesh_name, index_buffer, vertex_buffer));
        }
        meshes.insert_or_assign(m_index, std::move(mesh_handles));
    }
    return meshes;
}

void loadGLTFNodes(const GLTFDocument &doc, const std::vector<uint32_t> &node_indices,
                   const std::unordered_map<uint32_t, std::vector<MeshHandle>> &meshes,
                   const std::optional<NodeHandle> &parent_node, Scene *out_scene)
{
    for (const auto &node_index : node_indices)
    {
        const auto &node = doc.nodes.at(node_index);

        std::optional<NodeHandle> current_parent = std::nullopt;
        auto transform = node.matrix
                             ? Transform{node.matrix.value()}
                             : Transform{node.translation.value_or({0.0f, 0.0f, 0.0f}),
                                         node.rotation.value_or({0.0f, 0.0f, 0.0f, 1.0f}),
                                         node.scale.value_or({1.0f, 1.0f, 1.0f})};
        if (node.mesh && meshes.contains(node.mesh.value()))
        {
            const auto &mesh_handles = meshes.at(node.mesh.value());
            // Group meshes with multiple mesh primitives under a single node.
            const bool is_group = mesh_handles.size() > 1;
            std::optional<NodeHandle> group_handle = parent_node;
            if (is_group)
            {
                group_handle = out_scene->addNode(node.name.value_or(""), NodeFlags::NONE, transform, std::nullopt,
                                                  parent_node);
                // The children of the current node will have this grouped node as parent.
                current_parent = group_handle;
                transform = Transform{};
            }
            for (const auto &mesh_handle : mesh_handles)
            {
                const auto handle = out_scene->addNode(out_scene->getMeshName(mesh_handle), NodeFlags::NONE,
                                                       transform, mesh_handle,
                                                       group_handle);
                if (!current_parent)
                {
                    // If there's no grouped parent (which means a single mesh primitive), we use that mesh node as the
                    // parent for the children.
                    current_parent = handle;
                }
            }
        }
        else
        {
            const auto handle = out_scene->addNode(node.name.value_or(""), NodeFlags::NONE, transform, std::nullopt,
                                                   parent_node);
            current_parent = handle;
        }
        if (!node.children.empty())
        {
            loadGLTFNodes(doc, node.children, meshes, current_parent, out_scene);
        }
    }
}

inline bool loadGLTF(const SceneFileInfo &info, Scene *out_scene)
{
    auto loader = GLTFLoader(info.path, true, false);
    if (!loader.isValid())
    {
        return false;
    }
    const auto &doc = loader.getDocument();

    const auto &meshes = loadGLTFMeshes(loader, out_scene);
    if (!doc.scenes.empty())
    {
        // TODO: Add option to load multiple GLTF scenes.
        loadGLTFNodes(doc, doc.scenes[0].nodes, meshes, std::nullopt, out_scene);
    }
    return true;
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
        return loadGLTF(info, out_scene);
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