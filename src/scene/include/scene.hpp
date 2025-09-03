// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_SCENE_HPP
#define KIRANA_SCENE_SCENE_HPP


#include "node.hpp"
#include "camera.hpp"
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

    explicit Scene(const std::string &name) : m_name{name}
    {
    }

    const std::string &getName() const
    {
        return m_name;
    }

    void setName(const std::string &name)
    {
        m_name = name;
    }

    CameraHandle addPerspectiveCamera(const std::string &name, const std::array<float, 2> &clipping_planes,
                                      float fov_vertical, float aspect_ratio = 1.0f);
    CameraHandle addOrthographicCamera(const std::string &name, const std::array<float, 2> &clipping_planes, float size,
                                       float aspect_ratio = 1.0f);
    CameraHandle addOrthographicCamera(const std::string &name, const std::array<float, 2> &clipping_planes,
                                       const std::array<float, 2> &size_2d);

    MeshHandle addMesh(const std::string &name, const IndexBuffer &index_buffer, const VertexBuffer &vertex_buffer,
                       const std::optional<MaterialHandle> &material = std::nullopt);

    NodeHandle addNode(const std::string &name, NodeFlags flags, const Transform &transform,
                       const std::optional<std::variant<CameraHandle, LightHandle, MeshHandle>> &resource =
                           std::nullopt,
                       const std::optional<NodeHandle> &parent = std::nullopt);

    NodeHandle addPerspectiveCameraNode(const std::string &name, const std::array<float, 2> &clipping_planes,
                                        const float fov_vertical, const float aspect_ratio = 1.0f,
                                        const NodeFlags flags = NodeFlags::NONE,
                                        const Transform &transform = Transform{},
                                        const std::optional<NodeHandle> &parent = std::nullopt)
    {
        if (const auto cam_handle = addPerspectiveCamera(name, clipping_planes, fov_vertical, aspect_ratio);
            cam_handle.isValid())
        {
            return addNode(name, flags, transform, cam_handle, parent);
        }
        return NodeHandle{};
    }

    NodeHandle addOrthographicCameraNode(const std::string &name, const std::array<float, 2> &clipping_planes,
                                         const float size, const float aspect_ratio = 1.0f,
                                         const NodeFlags flags = NodeFlags::NONE,
                                         const Transform &transform = Transform{},
                                         const std::optional<NodeHandle> &parent = std::nullopt)
    {
        if (const auto cam_handle = addOrthographicCamera(name, clipping_planes, size, aspect_ratio);
            cam_handle.isValid())
        {
            return addNode(name, flags, transform, cam_handle, parent);
        }
        return NodeHandle{};
    }

    NodeHandle addOrthographicCameraNode(const std::string &name, const std::array<float, 2> &clipping_planes,
                                         const std::array<float, 2> &size_2d, const NodeFlags flags = NodeFlags::NONE,
                                         const Transform &transform = Transform{},
                                         const std::optional<NodeHandle> &parent = std::nullopt)
    {
        if (const auto cam_handle = addOrthographicCamera(name, clipping_planes, size_2d); cam_handle.isValid())
        {
            return addNode(name, flags, transform, cam_handle, parent);
        }
        return NodeHandle{};
    }

    NodeHandle addMeshNode(const std::string &name, const IndexBuffer &index_buffer, const VertexBuffer &vertex_buffer,
                           const std::optional<MaterialHandle> &material = std::nullopt,
                           const NodeFlags flags = NodeFlags::NONE, const Transform &transform = Transform{},
                           const std::optional<NodeHandle> &parent = std::nullopt)
    {
        if (const auto mesh_handle = addMesh(name, index_buffer, vertex_buffer, material); mesh_handle.isValid())
        {
            return addNode(name, flags, transform, mesh_handle, parent);
        }
        return NodeHandle{};
    }

    [[nodiscard]] std::string getCameraName(const CameraHandle handle) const
    {
        return m_camera_names.contains(handle) ? m_camera_names.at(handle) : "";
    }

    [[nodiscard]] const Camera *getCamera(const CameraHandle handle) const
    {
        return m_cameras.get(handle);
    }

    [[nodiscard]] Camera *getCamera(const CameraHandle handle)
    {
        return m_cameras.get(handle);
    }

    [[nodiscard]] const Camera *getCamera(const NodeHandle handle) const
    {
        if (const auto node = m_nodes.get(handle); node && node->getResourceType() == NodeType::CAMERA)
        {
            return m_cameras.get(std::get<CameraHandle>(node->resource.value()));
        }
        return nullptr;
    }

    [[nodiscard]] Camera *getCamera(const NodeHandle handle)
    {
        if (const auto node = m_nodes.get(handle); node && node->getResourceType() == NodeType::CAMERA)
        {
            return m_cameras.get(std::get<CameraHandle>(node->resource.value()));
        }
        return nullptr;
    }

    [[nodiscard]] const Camera *getActiveCamera() const
    {
        if (m_active_camera.isValid())
        {
            return getCamera(m_active_camera);
        }
        return nullptr;
    }

    [[nodiscard]] Camera *getActiveCamera()
    {
        if (m_active_camera.isValid())
        {
            return getCamera(m_active_camera);
        }
        return nullptr;
    }

    [[nodiscard]] Matrix4 getViewMatrix() const
    {
        if (const auto transform = getWorldTransform(m_active_camera_node); transform)
        {
            return transform->getLocalMatrix();
        }
        return Matrix4{};
    }

    [[nodiscard]] Matrix4 getProjectionMatrix() const
    {
        if (const auto cam = getActiveCamera(); cam)
        {
            return cam->getProjectionMatrix();
        }
        return Matrix4{};
    }

    void setActiveCamera(const NodeHandle handle)
    {
        if (const auto node = m_nodes.get(handle); node && node->getResourceType() == NodeType::CAMERA)
        {
            m_active_camera_node = handle;
            m_active_camera = std::get<CameraHandle>(node->resource.value());
        }
    }

    [[nodiscard]] std::string getMeshName(const MeshHandle handle) const
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

    [[nodiscard]] std::string getNodeName(const NodeHandle handle) const
    {
        return m_node_names.contains(handle) ? m_node_names.at(handle) : "";
    }

    [[nodiscard]] const Node *getNode(const NodeHandle handle) const
    {
        return m_nodes.get(handle);
    }

    [[nodiscard]] const HierarchyTransform *getTransform(const NodeHandle handle) const
    {
        if (m_node_transforms.contains(handle))
        {
            return m_transforms.get(m_node_transforms.at(handle));
        }
        return nullptr;
    }

    [[nodiscard]] const Transform *getWorldTransform(const NodeHandle handle) const
    {
        if (const auto transform = getTransform(handle); transform)
        {
            return &transform->world;
        }
        return nullptr;
    }

    [[nodiscard]] const Transform *getLocalTransform(const NodeHandle handle) const
    {
        if (const auto transform = getTransform(handle); transform)
        {
            return &transform->local;
        }
        return nullptr;
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
    std::string m_name{DEFAULT_NAME_SCENE};

    IndexBuffer m_index_buffer{};
    VertexBuffer m_vertex_buffer{};

    core::ResourceManager<Node, NodeTag> m_nodes;
    core::ResourceManager<HierarchyTransform, HierarchyTransformTag> m_transforms;
    std::unordered_map<NodeHandle, std::string> m_node_names;
    std::unordered_map<NodeHandle, HierarchyTransformHandle> m_node_transforms;

    core::ResourceManager<Camera, CameraTag> m_cameras;
    std::unordered_map<CameraHandle, std::string> m_camera_names;

    core::ResourceManager<Mesh, MeshTag> m_meshes;
    std::unordered_map<MeshHandle, std::string> m_mesh_names;

    CameraHandle m_active_camera{};
    NodeHandle m_active_camera_node{};

    CameraHandle addCamera(const std::string &name, const Camera &camera);
};
}
#endif //KIRANA_SCENE_SCENE_HPP