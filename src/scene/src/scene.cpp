// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "scene.hpp"

#include "gltf.hpp"

#include <logger.hpp>

namespace kirana::scene
{
ImageHandle Scene::addImage(const std::string &name, const Image &image)
{
    const auto handle = m_images.add(image);
    if (!handle.isValid())
    {
        return handle;
    }
    m_image_names.insert_or_assign(handle, name.empty()
                                               ? DEFAULT_NAME_IMAGE + "_" + std::to_string(handle.getIndex())
                                               : name);
    return handle;
}

ImageHandle Scene::addImage(const std::string &name, const std::string &path)
{
    return addImage(name, Image{path});
}

ImageHandle Scene::addImage(const std::string &name, const std::vector<uint8_t> &raw_buffer)
{
    return addImage(name, Image::loadFromRawBuffer(name, raw_buffer));
}

TextureHandle Scene::addTexture(const std::string &name, const ImageHandle image, const TextureSampler &sampler,
                                const TextureTransform &transform, const uint32_t tex_coord)
{
    const auto handle = m_textures.add(Texture{image, sampler, transform, tex_coord});
    if (!handle.isValid())
    {
        return handle;
    }
    m_texture_names.insert_or_assign(handle, name.empty()
                                                 ? DEFAULT_NAME_TEXTURE + "_" + std::to_string(handle.getIndex())
                                                 : name);
    return handle;
}

MaterialHandle Scene::addMaterial(const std::string &name, const MaterialPBR &material)
{
    const auto handle = m_materials.add(material);
    if (!handle.isValid())
    {
        return handle;
    }
    m_material_names.insert_or_assign(handle, name.empty()
                                                  ? DEFAULT_NAME_MATERIAL + "_" + std::to_string(handle.getIndex())
                                                  : name);
    return handle;
}

CameraHandle Scene::addCamera(const std::string &name, const Camera &camera)
{
    const auto handle = m_cameras.add(camera);
    if (!handle.isValid())
    {
        return handle;
    }
    m_camera_names.insert_or_assign(handle, name.empty()
                                                ? DEFAULT_NAME_CAMERA + "_" + std::to_string(handle.getIndex())
                                                : name);
    // Set the current camera as default if it's not set.
    if (!m_active_camera.isValid())
    {
        m_active_camera = handle;
    }
    return handle;
}

PunctualLightHandle Scene::addPunctualLight(const std::string &name, const PunctualLight &light)
{
    const auto handle = m_punctual_lights.add(light);
    if (!handle.isValid())
    {
        return handle;
    }
    m_punctual_light_names.insert_or_assign(handle, name.empty()
                                                        ? DEFAULT_NAME_LIGHT + "_" + std::to_string(handle.getIndex())
                                                        : name);
    return handle;
}

MeshHandle Scene::addMesh(const std::string &name, const IndexBuffer &index_buffer, const VertexBuffer &vertex_buffer,
                          const MaterialHandle material)
{
    if (vertex_buffer.isEmpty())
    {
        return MeshHandle{};
    }

    IndexBufferRange indices{};
    if (index_buffer.isEmpty())
    {
        std::vector<uint32_t> gen_indices;
        gen_indices.resize(vertex_buffer.positions.size());
        for (uint32_t i = 0; i < vertex_buffer.positions.size(); ++i)
        {
            gen_indices[i] = i;
        }
        indices = m_index_buffer.extend32(gen_indices);
    }
    else
    {
        if (!index_buffer.indices.empty())
        {
            indices = m_index_buffer.extend32(index_buffer.indices);
        }
        else if (!index_buffer.indices_16.empty())
        {
            indices = m_index_buffer.extend16(index_buffer.indices_16);
        }
        else if (!index_buffer.indices_8.empty())
        {
            indices = m_index_buffer.extend8(index_buffer.indices_8);
        }
    }

    const VertexBufferRange vertices = m_vertex_buffer.extend(vertex_buffer);

    const MeshHandle handle = m_meshes.add(Mesh{indices, vertices, material});
    if (!handle.isValid())
    {
        return handle;
    }

    m_mesh_names.insert_or_assign(handle, name.empty()
                                              ? DEFAULT_NAME_MESH + "_" + std::to_string(handle.getIndex())
                                              : name);

    return handle;
}

NodeHandle Scene::addNode(const std::string &name, const NodeFlags flags, const Transform &transform,
                          const std::optional<std::variant<CameraHandle, PunctualLightHandle, MeshHandle>> &resource,
                          const std::optional<NodeHandle> &parent)
{
    const auto handle = m_nodes.add(Node{flags});
    if (!handle.isValid())
    {
        return handle;
    }

    Node *node = m_nodes.get(handle);
    node->resource = resource;

    Transform global_transform = transform;
    // If the node has a parent, we modify the parent node and append this node in an appropriate child-sibling location.
    if (const auto parent_node = parent ? m_nodes.get(parent.value()) : nullptr; parent_node)
    {
        if (!parent_node->first_child || !m_nodes.isValid(parent_node->first_child.value()))
        {
            // If the parent node has no other children, append this node as its first child.
            parent_node->first_child = handle;
        }
        else
        {
            // If the parent node has child(ren), go through the sibling chain and append this node at the end.
            Node *sibling_node = m_nodes.get(parent_node->first_child.value());
            while (true)
            {
                if (!sibling_node->next_sibling || !m_nodes.isValid(sibling_node->next_sibling.value()))
                {
                    sibling_node->next_sibling = handle;
                    break;
                }
                sibling_node = m_nodes.get(sibling_node->next_sibling.value());
            }
        }
        node->parent = parent.value();
        node->level = parent_node->level + 1;
        if (const auto &t_handle = m_transforms.get(m_node_transforms.at(parent.value())); t_handle)
        {
            global_transform = t_handle->world * transform;
        }
    }
    m_node_names.insert_or_assign(handle, name.empty()
                                              ? DEFAULT_NAME_NODE + "_" + std::to_string(handle.getIndex())
                                              : name);

    const auto transform_handle = m_transforms.add(HierarchyTransform{transform, global_transform});
    m_node_transforms.insert_or_assign(handle, transform_handle);

    if (node->getResourceType() == NodeType::CAMERA)
    {
        // Set the default camera node if the given resource is a default camera.
        if (m_cameras.isValid(m_active_camera) && !m_active_camera_node.isValid())
        {
            m_active_camera_node = handle;
        }
    }

    return handle;
}
}