// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_SCENE_DATA_HPP
#define KIRANA_RENDERER_SCENE_DATA_HPP

#include "device.hpp"
#include <scene.hpp>

#include <map>

namespace kirana::renderer
{

const scene::Matrix4 VULKAN_PROJECTION_INVERT_Y{1.0f, 0.0f, 0.0f, 0.0f,
                                                0.0f, -1.0f, 0.0f, 0.0f,
                                                0.0f, 0.0f, 0.5f, 0.5f,
                                                0.0f, 0.0f, 0.0f, 1.0f};

struct TransformData
{
    std::array<float, 16> world{};
    std::array<float, 16> local{};
};

struct MeshData
{
    scene::IndexFormat index_format{};
    uint32_t index_offset{};
    uint32_t index_count{};
    uint32_t pos_offset{};
    uint32_t pos_count{};
    uint32_t normal_offset{};
    uint32_t normal_count{};
    uint32_t uv_offset{};
    uint32_t uv_count{};
    uint32_t color_offset{};
    uint32_t color_count{};
    uint32_t material_index{};
};

struct MeshInstanceData
{
    uint32_t mesh_index{};
    uint32_t transform_index{};
};

struct MeshInstancesData
{
    uint32_t instance_offset{};
    uint32_t instance_count{};
};

struct CameraData
{
    std::array<float, 16> view{};
    std::array<float, 16> projection{};
};

class SceneData
{
public:
    SceneData() = default;
    ~SceneData() = default;

    bool init(const Device &device, const scene::Scene &scene);
    void destroy();

    [[nodiscard]] bool hasIndexBuffer() const
    {
        return (m_buffer_index_8.isValid() && m_buffer_index_8.getSize() > 0)
               || (m_buffer_index_16.isValid() && m_buffer_index_16.getSize() > 0)
               || (m_buffer_index_32.isValid() && m_buffer_index_32.getSize() > 0);
    }

    [[nodiscard]] bool hasVertexBuffer() const
    {
        return m_buffer_position.isValid() && m_buffer_position.getSize() > 0;
    }

    [[nodiscard]] bool isValid() const
    {
        return m_fence.isValid() && m_encoder.isValid() && hasIndexBuffer() && hasVertexBuffer();
    }

    [[nodiscard]] const Buffer &getIndexBuffer8() const
    {
        return m_buffer_index_8;
    }

    [[nodiscard]] const Buffer &getIndexBuffer16() const
    {
        return m_buffer_index_16;
    }

    [[nodiscard]] const Buffer &getIndexBuffer32() const
    {
        return m_buffer_index_32;
    }

    [[nodiscard]] const Buffer &getPositionBuffer() const
    {
        return m_buffer_position;
    }

    [[nodiscard]] const Buffer &getNormalBuffer() const
    {
        return m_buffer_normal;
    }

    [[nodiscard]] const Buffer &getUVBuffer() const
    {
        return m_buffer_uv;
    }

    [[nodiscard]] const Buffer &getColorBuffer() const
    {
        return m_buffer_color;
    }

    [[nodiscard]] const Buffer &getTransformsBuffer() const
    {
        return m_buffer_transforms;
    }

    [[nodiscard]] const Buffer &getMeshesBuffer() const
    {
        return m_buffer_meshes;
    }

    [[nodiscard]] const Buffer &getMeshInstancesBuffer() const
    {
        return m_buffer_mesh_instances;
    }

    [[nodiscard]] const Buffer &getCameraBuffer() const
    {
        return m_buffer_camera;
    }

    void forEachRenderable(const std::function<void(const MeshData &, const MeshInstancesData &)> &callback) const
    {
        for (const auto &[mesh_index, instances] : m_mesh_instance_map)
        {
            callback(m_meshes.at(mesh_index), instances);
        }
    }

    bool updateCamera(const Device &device, const scene::Matrix4 &view_matrix, const scene::Matrix4 &projection_matrix);

private:
    Fence m_fence{};
    CommandEncoder m_encoder{};
    // Index buffers
    Buffer m_buffer_index_8{};
    Buffer m_buffer_index_16{};
    Buffer m_buffer_index_32{};
    // Vertex buffers
    Buffer m_buffer_position{};
    Buffer m_buffer_normal{};
    Buffer m_buffer_uv{};
    Buffer m_buffer_color{};
    // Mesh Data buffers
    std::vector<TransformData> m_transforms{};
    std::vector<MeshData> m_meshes{};
    std::vector<MeshInstanceData> m_mesh_instances{};
    std::map<uint32_t, MeshInstancesData> m_mesh_instance_map{};
    Buffer m_buffer_transforms{};
    Buffer m_buffer_meshes{};
    Buffer m_buffer_mesh_instances{};
    // Camera buffer
    CameraData m_camera{};
    Buffer m_buffer_camera{};
};
}

#endif //KIRANA_RENDERER_SCENE_DATA_HPP