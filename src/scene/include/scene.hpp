// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_SCENE_HPP
#define KIRANA_SCENE_SCENE_HPP

#include "image.hpp"
#include "texture.hpp"
#include "material.hpp"
#include "node.hpp"
#include "camera.hpp"
#include "light.hpp"
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

    explicit Scene(std::string name) : m_name{std::move(name)}
    {
    }

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
    }

    void setName(const std::string &name)
    {
        m_name = name;
    }


    ImageHandle addImage(const std::string &name, const Image &image);
    ImageHandle addImage(const std::string &name, const std::string &path);
    ImageHandle addImage(const std::string &name, const std::vector<uint8_t> &raw_buffer);

    TextureHandle addTexture(const std::string &name, ImageHandle image, const TextureSampler &sampler = {},
                             const TextureTransform &transform = {}, uint32_t tex_coord = 0u);

    MaterialHandle addMaterial(const std::string &name, const MaterialPBR &material);

    CameraHandle addCamera(const std::string &name, const Camera &camera);

    CameraHandle addPerspectiveCamera(const std::string &name, const std::array<float, 2> &clipping_planes,
                                      const float fov_vertical, const float aspect_ratio = 1.0f)
    {
        return addCamera(name,
                         Camera::getPerspective(clipping_planes[0], clipping_planes[1], fov_vertical, aspect_ratio));
    }

    CameraHandle addOrthographicCamera(const std::string &name, const std::array<float, 2> &clipping_planes,
                                       const float size, const float aspect_ratio = 1.0f)
    {
        return addCamera(name,
                         Camera::getOrthographic(clipping_planes[0], clipping_planes[1], size, aspect_ratio));
    }

    CameraHandle addOrthographicCamera(const std::string &name, const std::array<float, 2> &clipping_planes,
                                       const std::array<float, 2> &size_2d)
    {
        return addCamera(name,
                         Camera::getOrthographicFromSize2D(clipping_planes[0], clipping_planes[1], size_2d[0],
                                                           size_2d[1]));
    }

    void setEnvironmentLight(const EnvironmentLight &light)
    {
        m_environment_light = light;
    }

    TextureHandle setEnvironmentLightImage(const Image &image, const TextureSampler &sampler = {},
                                           const TextureTransform &transform = {})
    {
        if (const auto tex = getTexture(m_environment_light.texture); tex)
        {
            // If the environment light already has a texture assigned, replace it with the provided one.
            if (m_images.isValid(tex->image))
            {
                m_images.remove(tex->image);
            }
            tex->image = addImage("Image_Environment_Light", image);
            tex->sampler = sampler;
            tex->transform = transform;
            return m_environment_light.texture;
        }
        const auto img_handle = addImage("Image_Environment_Light", image);
        m_environment_light.texture = addTexture("Texture_Environment_Light", img_handle, sampler, transform);
        return m_environment_light.texture;
    }

    PunctualLightHandle addPunctualLight(const std::string &name, const PunctualLight &light);

    PunctualLightHandle addDirectionalLight(const std::string &name, const std::array<float, 3> &color,
                                            const float intensity,
                                            const LightUnit unit = DEFAULT_LIGHT_UNITS.at(
                                                PunctualLightType::DIRECTIONAL))
    {
        return addPunctualLight(name, PunctualLight{PunctualLightType::DIRECTIONAL, unit, color, intensity});
    }

    PunctualLightHandle addPointLight(const std::string &name, const std::array<float, 3> &color, const float intensity,
                                      const float range = -1.0f,
                                      const LightUnit unit = DEFAULT_LIGHT_UNITS.at(PunctualLightType::POINT))
    {
        return addPunctualLight(name, PunctualLight{PunctualLightType::POINT, unit, color, intensity, range});
    }

    PunctualLightHandle addSpotLight(const std::string &name, const std::array<float, 3> &color, const float intensity,
                                     const float range = -1.0f, const float inner_cone_angle = 0.0f,
                                     const float outer_cone_angle = 0.78539816339f,
                                     const LightUnit unit = DEFAULT_LIGHT_UNITS.at(PunctualLightType::SPOT))
    {
        return addPunctualLight(name, PunctualLight{PunctualLightType::SPOT, unit, color, intensity, range,
                                                    inner_cone_angle, outer_cone_angle});
    }

    MeshHandle addMesh(const std::string &name, const IndexBuffer &index_buffer, const VertexBuffer &vertex_buffer,
                       MaterialHandle material);

    NodeHandle addNode(const std::string &name, NodeFlags flags, const Transform &transform,
                       const std::optional<std::variant<CameraHandle, PunctualLightHandle, MeshHandle>> &resource =
                           std::nullopt,
                       const std::optional<NodeHandle> &parent = std::nullopt);

    NodeHandle addCameraNode(const std::string &name, const Camera &camera, const NodeFlags flags = NodeFlags::NONE,
                             const Transform &transform = Transform{},
                             const std::optional<NodeHandle> &parent = std::nullopt)
    {
        if (const auto cam_handle = addCamera(name, camera);
            cam_handle.isValid())
        {
            return addNode(name, flags, transform, cam_handle, parent);
        }
        return NodeHandle{};
    }

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

    NodeHandle addPunctualLightNode(const std::string &name, const PunctualLight &light,
                                    const NodeFlags flags = NodeFlags::NONE,
                                    const Transform &transform = Transform{},
                                    const std::optional<NodeHandle> &parent = std::nullopt)
    {
        if (const auto light_handle = addPunctualLight(name, light); light_handle.isValid())
        {
            return addNode(name, flags, transform, light_handle, parent);
        }
        return NodeHandle{};
    }

    NodeHandle addDirectionalLightNode(const std::string &name, const std::array<float, 3> &color,
                                       const float intensity,
                                       const LightUnit unit = DEFAULT_LIGHT_UNITS.at(PunctualLightType::DIRECTIONAL),
                                       const NodeFlags flags = NodeFlags::NONE,
                                       const Transform &transform = Transform{},
                                       const std::optional<NodeHandle> &parent = std::nullopt)
    {
        if (const auto light_handle = addDirectionalLight(name, color, intensity, unit); light_handle.isValid())
        {
            return addNode(name, flags, transform, light_handle, parent);
        }
        return NodeHandle{};
    }

    NodeHandle addPointLightNode(const std::string &name, const std::array<float, 3> &color,
                                 const float intensity, const float range = -1.0f,
                                 const LightUnit unit = DEFAULT_LIGHT_UNITS.at(PunctualLightType::POINT),
                                 const NodeFlags flags = NodeFlags::NONE,
                                 const Transform &transform = Transform{},
                                 const std::optional<NodeHandle> &parent = std::nullopt)
    {
        if (const auto light_handle = addPointLight(name, color, intensity, range, unit); light_handle.isValid())
        {
            return addNode(name, flags, transform, light_handle, parent);
        }
        return NodeHandle{};
    }

    NodeHandle addSpotLightNode(const std::string &name, const std::array<float, 3> &color,
                                const float intensity, const float range = -1.0f, const float inner_cone_angle = 0.0f,
                                const float outer_cone_angle = 0.78539816339f,
                                const LightUnit unit = DEFAULT_LIGHT_UNITS.at(PunctualLightType::SPOT),
                                const NodeFlags flags = NodeFlags::NONE,
                                const Transform &transform = Transform{},
                                const std::optional<NodeHandle> &parent = std::nullopt)
    {
        if (const auto light_handle = addSpotLight(name, color, intensity, range, inner_cone_angle, outer_cone_angle,
                                                   unit); light_handle.isValid())
        {
            return addNode(name, flags, transform, light_handle, parent);
        }
        return NodeHandle{};
    }

    NodeHandle addMeshNode(const std::string &name, const IndexBuffer &index_buffer, const VertexBuffer &vertex_buffer,
                           const MaterialHandle material, const NodeFlags flags = NodeFlags::NONE,
                           const Transform &transform = Transform{},
                           const std::optional<NodeHandle> &parent = std::nullopt)
    {
        if (const auto mesh_handle = addMesh(name, index_buffer, vertex_buffer, material); mesh_handle.isValid())
        {
            return addNode(name, flags, transform, mesh_handle, parent);
        }
        return NodeHandle{};
    }

    [[nodiscard]] const Texture *getTexture(const TextureHandle handle) const
    {
        return m_textures.get(handle);
    }

    [[nodiscard]] Texture *getTexture(const TextureHandle handle)
    {
        return m_textures.get(handle);
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

    [[nodiscard]] std::string getPunctualLightName(const PunctualLightHandle handle) const
    {
        return m_punctual_light_names.contains(handle) ? m_punctual_light_names.at(handle) : "";
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

    core::ResourceManager<Image, ImageTag> m_images;
    std::unordered_map<ImageHandle, std::string> m_image_names;
    core::ResourceManager<Texture, TextureTag> m_textures;
    std::unordered_map<TextureHandle, std::string> m_texture_names;
    core::ResourceManager<MaterialPBR, MaterialTag> m_materials;
    std::unordered_map<MaterialHandle, std::string> m_material_names;

    core::ResourceManager<Camera, CameraTag> m_cameras;
    std::unordered_map<CameraHandle, std::string> m_camera_names;

    EnvironmentLight m_environment_light{};
    core::ResourceManager<PunctualLight, PunctualLightTag> m_punctual_lights;
    std::unordered_map<PunctualLightHandle, std::string> m_punctual_light_names;

    core::ResourceManager<Mesh, MeshTag> m_meshes;
    std::unordered_map<MeshHandle, std::string> m_mesh_names;

    core::ResourceManager<Node, NodeTag> m_nodes;
    core::ResourceManager<HierarchyTransform, HierarchyTransformTag> m_transforms;
    std::unordered_map<NodeHandle, std::string> m_node_names;
    std::unordered_map<NodeHandle, HierarchyTransformHandle> m_node_transforms;

    CameraHandle m_active_camera{};
    NodeHandle m_active_camera_node{};
};
}
#endif //KIRANA_SCENE_SCENE_HPP