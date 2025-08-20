// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_SCENE_HPP
#define KIRANA_SCENE_SCENE_HPP


#include "node.hpp"
#include "mesh.hpp"
#include "transform.hpp"

#include <no_copy.hpp>

#include <functional>
#include <optional>
#include <unordered_map>

namespace kirana::scene
{
using RenderableCallback = std::function<void(NodeHandle node_handle, MeshHandle mesh_handle,
                                              const HierarchyTransform &transform, const Mesh &mesh)>;

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

    [[nodiscard]] std::string getNodeName(const NodeHandle &handle) const
    {
        return m_node_names.contains(handle) ? m_node_names.at(handle) : "";
    }

    [[nodiscard]] std::string getMeshName(const MeshHandle &handle) const
    {
        return m_mesh_names.contains(handle) ? m_mesh_names.at(handle) : "";
    }

    [[nodiscard]] const IndexBuffer &getIndexBuffer() const
    {
        return m_index_buffer;
    }

    [[nodiscard]] const VertexBuffer &getVertexBuffer() const
    {
        return m_vertex_buffer;
    }

    [[nodiscard]] const Node *getNode(const NodeHandle handle) const
    {
        return m_nodes.get(handle);
    }

    void forEachRenderables(const RenderableCallback &callback) const
    {
        m_nodes.forEach([&](const NodeHandle handle, const Node &node) {
            if (!hasFlag(node.flags, NodeFlags::HIDDEN) && node.getResourceType() == NodeType::MESH)
            {
                const HierarchyTransform &transform = *m_transforms.get(m_node_transforms.at(handle));
                const MeshHandle mesh_handle = std::get<MeshHandle>(node.resource.value());
                const Mesh &mesh = *m_meshes.get(mesh_handle);
                callback(handle, mesh_handle, transform, mesh);
            }
        });
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