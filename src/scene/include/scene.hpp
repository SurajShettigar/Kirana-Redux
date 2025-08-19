// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_SCENE_HPP
#define KIRANA_SCENE_SCENE_HPP

#include "node.hpp"
#include "mesh.hpp"
#include "transform.hpp"

#include <no_copy.hpp>

#include <unordered_map>
#include <optional>

namespace kirana::scene
{
class Scene : public core::NoCopy
{
public:
    Scene() = default;
    ~Scene() = default;

    NodeHandle addNode(const std::string &name, NodeFlags flags, const Transform &transform,
                       const std::optional<std::variant<CameraHandle, LightHandle, MeshHandle>> &resource =
                           std::nullopt,
                       const std::optional<NodeHandle> &parent = std::nullopt);

    MeshHandle addMesh(const std::string &name, const IndexBuffer &index_buffer, const VertexBuffer &vertex_buffer,
                       const std::optional<MaterialHandle> &material = std::nullopt);

    [[nodiscard]] std::string getMeshName(const MeshHandle &handle) const
    {
        return m_mesh_names.contains(handle) ? m_mesh_names.at(handle) : "";
    }
private:
    IndexBuffer m_index_buffer{};
    VertexBuffer m_vertex_buffer{};

    core::ResourceManager<Node, NodeTag> m_nodes;
    core::ResourceManager<HierarchyTransform, HierarchyTransformTag> m_transforms;
    std::unordered_map<NodeHandle, std::string> m_node_names;
    std::unordered_map<NodeHandle, HierarchyTransformHandle> m_node_transforms;

    core::ResourceManager<Mesh, MeshTag> m_meshes;
    std::unordered_map<MeshHandle, std::string> m_mesh_names;

    std::string m_name;
};
}
#endif //KIRANA_SCENE_SCENE_HPP