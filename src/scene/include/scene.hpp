// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_SCENE_HPP
#define KIRANA_SCENE_SCENE_HPP

#include "common.hpp"

#include <no_copy.hpp>
#include <resource_manager.hpp>

#include <unordered_map>
#include <variant>
#include <span>

namespace kirana::scene
{
struct VertexBuffer
{
    std::vector<INDEX_FORMAT> indices;
    std::vector<VECTOR_2> uvs;
    std::vector<VECTOR_4> positions;
    std::vector<VECTOR_4> normals;
    std::vector<VECTOR_4> colors;
};

struct Transform
{
    MATRIX_4 local_to_world;
};

enum class NodeType: uint8_t
{
    EMPTY = 0u,
    CAMERA = 1u,
    LIGHT = 2u,
    MESH = 3u,
    COUNT = 4u,
};

enum class NodeFlags: uint32_t
{
    NONE = 0u,
    HIDDEN = 1u << 0u,
    SELECTED = 1u << 1u,
};

struct Node final : core::IResource
{
    NodeFlags flags{NodeFlags::NONE};
    std::optional<NodeHandle> parent{std::nullopt};
    std::optional<NodeHandle> first_child{std::nullopt};
    std::optional<NodeHandle> next_sibling{std::nullopt};
    uint32_t level{0};

    std::optional<std::variant<CameraHandle, LightHandle, MeshHandle>> resource{std::nullopt};

    NodeType getNodeType() const
    {
        return resource ? static_cast<NodeType>(resource.value().index() + 1) : NodeType::EMPTY;
    }

    Node() = default;

    Node(NodeFlags flags, std::optional<NodeHandle> parent, std::optional<NodeHandle> first_child,
         std::optional<NodeHandle> next_sibling, uint32_t level,
         const std::optional<std::variant<CameraHandle, LightHandle, MeshHandle>> &resource) : IResource(),
        flags(flags),
        parent(parent), first_child(first_child), next_sibling(next_sibling), level(level), resource(resource)
    {
    }
};

struct Mesh final : core::IResource
{
    std::span<INDEX_FORMAT> indices;
    std::span<VECTOR_2> uvs;
    std::span<VECTOR_4> positions;
    std::span<VECTOR_4> normals;
    std::span<VECTOR_4> colors;

    MaterialHandle material;
};

class Scene : public core::NoCopy
{
public:
    Scene() = default;
    ~Scene() = default;

    NodeHandle addNode(const std::string &name, const NodeFlags flags, const Transform &transform,
                       const std::optional<std::variant<CameraHandle, LightHandle, MeshHandle>> &resource =
                           std::nullopt,
                       const std::optional<NodeHandle> &parent = std::nullopt)
    {
        const auto handle = m_nodes.add(Node{flags, parent, std::nullopt, std::nullopt, 0, resource});
        if (!handle.isValid())
        {
            return handle;
        }
        m_node_transforms.insert_or_assign(handle, transform);
        if (!name.empty())
        {
            m_node_names.insert_or_assign(handle, name);
        }
        return handle;
    }

    MeshHandle addMesh(const std::string &name)
    {
        const MeshHandle handle = m_meshes.add(Mesh{});
        if (!handle.isValid())
        {
            return handle;
        }
        if (!name.empty())
        {
            m_mesh_names.insert_or_assign(handle, name);
        }
        return handle;
    }

private:
    VertexBuffer m_vertex_buffer{};

    core::ResourceManager<Node, NodeTag> m_nodes;
    std::unordered_map<NodeHandle, Transform> m_node_transforms;
    std::unordered_map<NodeHandle, std::string> m_node_names;

    core::ResourceManager<Mesh, MeshTag> m_meshes;
    std::unordered_map<MeshHandle, std::string> m_mesh_names;

    std::string m_name;
};
}
#endif //KIRANA_SCENE_SCENE_HPP