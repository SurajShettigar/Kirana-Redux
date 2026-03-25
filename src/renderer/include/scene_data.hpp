// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_SCENE_DATA_HPP
#define KIRANA_RENDERER_SCENE_DATA_HPP

#include "device.hpp"
#include <scene.hpp>

#include <map>

namespace kirana::renderer
{

const scene::Matrix4 VULKAN_PROJECTION_INVERT_Y{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                                                0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f,  0.0f, 1.0f};

struct TextureData
{
    std::array<float, 2> offset{};
    std::array<float, 2> scale{};

    float rotation{};
    uint32_t uv_index{};
    uint32_t texture_index{};
    uint32_t sampler_index{};
};

struct TransformData
{
    std::array<float, 16> world{};
    std::array<float, 16> local{};
};

struct PunctualLightData
{
    std::array<float, 3> color{};
    scene::PunctualLightType type{};

    float intensity{};
    float range{};
    float spot_cone_angle_inner{};
    float spot_cone_angle_outer{};

    std::array<uint32_t, 3> _padding{};
    scene::LightUnit unit{};
};

struct EnvironmentLightData
{
    std::array<float, 3> color{};
    float intensity{};

    std::array<float, 16> rotation_matrix{};

    scene::LightUnit unit{};
    uint32_t texture_index{};
};

struct MaterialPBRData
{
    std::array<float, 4> base_color{};

    std::array<float, 3> specular_color{};
    float specular_factor{};

    std::array<float, 3> emissive_color{};
    float emissive_strength{};

    std::array<float, 3> sheen_color{};
    float sheen_roughness_factor{};

    float metallic_factor{};
    float roughness_factor{};
    float anisotropy_strength{};
    float anisotropy_rotation{};

    float iridescence_factor{};
    float iridescence_ior{};
    float iridescence_thickness_min{};
    float iridescence_thickness_max{};

    std::array<float, 3> diffuse_transmission_color{};
    float diffuse_transmission_factor{};

    std::array<float, 3> volume_attenuation_color{};
    float volume_thickness_factor{};

    float volume_attenuation_distance{};
    float dispersion_factor{};
    float transmission_factor{};
    float ior{};

    float clearcoat_factor{};
    float clearcoat_roughness_factor{};
    float normal_scale{};
    float occlusion_strength{};

    uint32_t texture_base_color_index{};
    uint32_t texture_specular_color_index{};
    /// A - Specular factor.
    uint32_t texture_specular_index{};
    uint32_t texture_emissive_color_index{};

    uint32_t texture_sheen_color_index{};
    /// A - Sheen roughness factor.
    uint32_t texture_sheen_roughness_index{};
    /// R - Undefined, G - Roughness, B - Metallic
    uint32_t texture_metallic_roughness_index{};
    /// RG - Tangent, Bitangent rotation direction (Convert [0, 1] to -[1, 1] range). B - Anisotropy strength.
    uint32_t texture_anisotropy_index{};

    /// R - Iridescence factor.
    uint32_t texture_iridescence_index{};
    /// G - Iridescence thickness. Value is used to lerp from min-max thickness values.
    uint32_t texture_iridescence_thickness_index{};
    uint32_t texture_diffuse_transmission_color_index{};
    /// A - Diffuse transmission factor.
    uint32_t texture_diffuse_transmission_index{};

    /// G - Volume thickness factor.
    uint32_t texture_volume_thickness_index{};
    uint32_t texture_transmission_index{};
    /// R - Clearcoat strength.
    uint32_t texture_clearcoat_index{};
    /// G - Clearcoat roughness.
    uint32_t texture_clearcoat_roughness_index{};

    uint32_t texture_clearcoat_normal_index{};
    uint32_t texture_normal_index{};
    uint32_t texture_occlusion_index{};
    scene::AlphaMode alpha_mode{};

    float alpha_cutoff{};
    bool double_sided{};
    std::array<uint32_t, 2> _padding{};
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

struct InstanceData
{
    uint32_t data_index{};
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

    [[nodiscard]] bool hasTextures() const
    {
        return !m_textures.empty() && m_buffer_texture_data.isValid() && !m_texture_data.empty();
    }

    [[nodiscard]] const std::vector<Texture> &getTextures() const
    {
        return m_textures;
    }

    [[nodiscard]] const std::vector<TextureSampler> &getTextureSamplers() const
    {
        return m_texture_samplers;
    }

    [[nodiscard]] const BufferHandle &getTextureDataBuffer() const
    {
        return m_buffer_texture_data;
    }

    [[nodiscard]] const BufferHandle &getTransformsBuffer() const
    {
        return m_buffer_transforms;
    }

    [[nodiscard]] bool hasEnvironmentLight() const
    {
        return m_buffer_environment_light.isValid();
    }

    [[nodiscard]] const BufferHandle &getEnvironmentLightBuffer() const
    {
        return m_buffer_environment_light;
    }

    [[nodiscard]] bool hasEnvironmentLightTexture() const
    {
        return m_environment_light.texture_index < std::numeric_limits<uint32_t>::max();
    }

    [[nodiscard]] const Texture &getEnvironmentLightTexture() const
    {
        return m_textures[m_texture_data[m_environment_light.texture_index].texture_index];
    }

    [[nodiscard]] const TextureSampler &getEnvironmentLightTextureSampler() const
    {
        return m_texture_samplers[m_texture_data[m_environment_light.texture_index].sampler_index];
    }

    [[nodiscard]] bool hasPunctualLights() const
    {
        return m_buffer_punctual_lights.isValid() && m_buffer_punctual_light_instances.isValid() &&
               !m_punctual_lights.empty() && !m_punctual_light_instances.empty();
    }

    [[nodiscard]] const BufferHandle &getPunctualLightsBuffer() const
    {
        return m_buffer_punctual_lights;
    }

    [[nodiscard]] const BufferHandle &getPunctualLightInstancesBuffer() const
    {
        return m_buffer_punctual_light_instances;
    }

    [[nodiscard]] bool hasMaterials() const
    {
        return m_buffer_materials.isValid() && m_materials.size() > 0;
    }

    [[nodiscard]] const BufferHandle &getMaterialsBuffer() const
    {
        return m_buffer_materials;
    }

    [[nodiscard]] bool hasIndexBuffer() const
    {
        return (m_buffer_index_8.isValid()/* && m_buffer_index_8.getSize() > 0*/) ||
               (m_buffer_index_16.isValid()/* && m_buffer_index_16.getSize() > 0*/) ||
               (m_buffer_index_32.isValid()/* && m_buffer_index_32.getSize() > 0*/);
    }

    [[nodiscard]] bool hasVertexBuffer() const
    {
        return m_buffer_position.isValid()/* && m_buffer_position.getSize() > 0*/;
    }

    [[nodiscard]] bool isValid() const
    {
        const bool has_mesh_data = hasIndexBuffer() && hasVertexBuffer();
        return m_fence.isValid() && m_encoder.isValid() && has_mesh_data;
    }

    [[nodiscard]] const BufferHandle &getIndexBuffer8() const
    {
        return m_buffer_index_8;
    }

    [[nodiscard]] const BufferHandle &getIndexBuffer16() const
    {
        return m_buffer_index_16;
    }

    [[nodiscard]] const BufferHandle &getIndexBuffer32() const
    {
        return m_buffer_index_32;
    }

    [[nodiscard]] const BufferHandle &getPositionBuffer() const
    {
        return m_buffer_position;
    }

    [[nodiscard]] const BufferHandle &getNormalBuffer() const
    {
        return m_buffer_normal;
    }

    [[nodiscard]] const BufferHandle &getUVBuffer() const
    {
        return m_buffer_uv;
    }

    [[nodiscard]] const BufferHandle &getColorBuffer() const
    {
        return m_buffer_color;
    }

    [[nodiscard]] const BufferHandle &getMeshesBuffer() const
    {
        return m_buffer_meshes;
    }

    [[nodiscard]] const BufferHandle &getMeshInstancesBuffer() const
    {
        return m_buffer_mesh_instances;
    }

    [[nodiscard]] const BufferHandle &getCameraBuffer() const
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

    std::vector<Texture> m_textures{};
    std::vector<TextureSampler> m_texture_samplers{};
    std::vector<TextureData> m_texture_data{};
    BufferHandle m_buffer_texture_data{};

    std::vector<TransformData> m_transforms{};
    BufferHandle m_buffer_transforms{};

    EnvironmentLightData m_environment_light{};
    BufferHandle m_buffer_environment_light{};

    std::vector<PunctualLightData> m_punctual_lights{};
    BufferHandle m_buffer_punctual_lights{};
    std::vector<InstanceData> m_punctual_light_instances{};
    BufferHandle m_buffer_punctual_light_instances{};

    std::vector<MaterialPBRData> m_materials{};
    BufferHandle m_buffer_materials{};

    // Index buffers
    BufferHandle m_buffer_index_8{};
    BufferHandle m_buffer_index_16{};
    BufferHandle m_buffer_index_32{};
    // Vertex buffers
    BufferHandle m_buffer_position{};
    BufferHandle m_buffer_normal{};
    BufferHandle m_buffer_uv{};
    BufferHandle m_buffer_color{};
    // Mesh Data buffers
    std::vector<MeshData> m_meshes{};
    BufferHandle m_buffer_meshes{};
    std::vector<InstanceData> m_mesh_instances{};
    std::map<uint32_t, MeshInstancesData> m_mesh_instance_map{}; // (mesh index, mesh instances)
    BufferHandle m_buffer_mesh_instances{};

    // Camera buffer
    CameraData m_camera{};
    BufferHandle m_buffer_camera{};

    uint32_t addTextureSampler(const Device &device, const scene::TextureSampler &sampler);
    uint32_t addMesh(const scene::MeshHandle &handle, const scene::Mesh &mesh,
                     const std::unordered_map<scene::MaterialHandle, uint32_t> &material_indices,
                     std::unordered_map<scene::MeshHandle, uint32_t> &out_mesh_indices);
};
} // namespace kirana::renderer

#endif // KIRANA_RENDERER_SCENE_DATA_HPP