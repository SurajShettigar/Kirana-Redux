// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "scene_data.hpp"

#include "helpers_vulkan.hpp"

#include <logger.hpp>

namespace kirana::renderer
{

inline TextureFormat getTextureFormatForImage(const scene::ImageChannelFormat channel_format,
                                              const uint32_t num_channels, const bool is_srgb)
{
    switch (channel_format)
    {
    case scene::ImageChannelFormat::UINT_8: {
        switch (num_channels)
        {
        case 1:
            return is_srgb ? TextureFormat::R8_SRGB : TextureFormat::R8_UNORM;
        case 2:
            return is_srgb ? TextureFormat::R8G8_SRGB : TextureFormat::R8G8_UNORM;
        case 3:
            return is_srgb ? TextureFormat::R8G8B8_SRGB : TextureFormat::R8G8B8_UNORM;
        default:
            return is_srgb ? TextureFormat::R8G8B8A8_SRGB : TextureFormat::R8G8B8A8_UNORM;
        }
    }
    case scene::ImageChannelFormat::INT_8: {
        switch (num_channels)
        {
        case 1:
            return TextureFormat::R8_SNORM;
        case 2:
            return TextureFormat::R8G8_SNORM;
        case 3:
            return TextureFormat::R8G8B8_SNORM;
        default:
            return TextureFormat::R8G8B8A8_SNORM;
        }
    }
    case scene::ImageChannelFormat::UINT_16: {
        switch (num_channels)
        {
        case 1:
            return TextureFormat::R16_UNORM;
        case 2:
            return TextureFormat::R16G16_UNORM;
        case 3:
            return TextureFormat::R16G16B16_UNORM;
        default:
            return TextureFormat::R16G16B16A16_UNORM;
        }
    }
    case scene::ImageChannelFormat::INT_16: {
        switch (num_channels)
        {
        case 1:
            return TextureFormat::R16_SNORM;
        case 2:
            return TextureFormat::R16G16_SNORM;
        case 3:
            return TextureFormat::R16G16B16_SNORM;
        default:
            return TextureFormat::R16G16B16A16_SNORM;
        }
    }
    case scene::ImageChannelFormat::UINT_32: {
        switch (num_channels)
        {
        case 1:
            return TextureFormat::R32_UINT;
        case 2:
            return TextureFormat::R32G32_UINT;
        case 3:
            return TextureFormat::R32G32B32_UINT;
        default:
            return TextureFormat::R32G32B32A32_UINT;
        }
    }
    case scene::ImageChannelFormat::INT_32: {
        switch (num_channels)
        {
        case 1:
            return TextureFormat::R32_SINT;
        case 2:
            return TextureFormat::R32G32_SINT;
        case 3:
            return TextureFormat::R32G32B32_SINT;
        default:
            return TextureFormat::R32G32B32A32_SINT;
        }
    }
    case scene::ImageChannelFormat::UINT_64: {
        switch (num_channels)
        {
        case 1:
            return TextureFormat::R64_UINT;
        case 2:
            return TextureFormat::R64G64_UINT;
        case 3:
            return TextureFormat::R64G64B64_UINT;
        default:
            return TextureFormat::R64G64B64A64_UINT;
        }
    }
    case scene::ImageChannelFormat::INT_64: {
        switch (num_channels)
        {
        case 1:
            return TextureFormat::R64_SINT;
        case 2:
            return TextureFormat::R64G64_SINT;
        case 3:
            return TextureFormat::R64G64B64_SINT;
        default:
            return TextureFormat::R64G64B64A64_SINT;
        }
    }
    case scene::ImageChannelFormat::HALF: {
        switch (num_channels)
        {
        case 1:
            return TextureFormat::R16_SFLOAT;
        case 2:
            return TextureFormat::R16G16_SFLOAT;
        case 3:
            return TextureFormat::R16G16B16_SFLOAT;
        default:
            return TextureFormat::R16G16B16A16_SFLOAT;
        }
    }
    case scene::ImageChannelFormat::FLOAT: {
        switch (num_channels)
        {
        case 1:
            return TextureFormat::R32_SFLOAT;
        case 2:
            return TextureFormat::R32G32_SFLOAT;
        case 3:
            return TextureFormat::R32G32B32_SFLOAT;
        default:
            return TextureFormat::R32G32B32A32_SFLOAT;
        }
    }
    case scene::ImageChannelFormat::DOUBLE: {
        switch (num_channels)
        {
        case 1:
            return TextureFormat::R64_SFLOAT;
        case 2:
            return TextureFormat::R64G64_SFLOAT;
        case 3:
            return TextureFormat::R64G64B64_SFLOAT;
        default:
            return TextureFormat::R64G64B64A64_SFLOAT;
        }
    }
    default:
        return TextureFormat::UNKNOWN;
    }
}

uint32_t SceneData::addTextureSampler(const Device &device, const scene::TextureSampler &sampler)
{
    const auto getFilterMode = [](const scene::TextureFilterMode mode) -> SamplerFilterMode {
        switch (mode)
        {
        case scene::TextureFilterMode::LINEAR:
            return SamplerFilterMode::LINEAR;
        case scene::TextureFilterMode::NEAREST:
        default:
            return SamplerFilterMode::NEAREST;
        }
    };
    const auto getWrapMode = [](const scene::TextureWrapMode mode) -> SamplerWrapMode {
        switch (mode)
        {
        case scene::TextureWrapMode::MIRRORED_REPEAT:
            return SamplerWrapMode::MIRRORED_REPEAT;
        case scene::TextureWrapMode::CLAMP_TO_EDGE:
            return SamplerWrapMode::CLAMP_TO_EDGE;
        case scene::TextureWrapMode::CLAMP_TO_BORDER:
            return SamplerWrapMode::CLAMP_TO_BORDER;
        case scene::TextureWrapMode::MIRRORED_CLAMP_TO_EDGE:
            return SamplerWrapMode::MIRROR_CLAMP_TO_EDGE;
        case scene::TextureWrapMode::REPEAT:
        default:
            return SamplerWrapMode::REPEAT;
        }
    };
    const SamplerData data{getFilterMode(sampler.mag_filter),   getFilterMode(sampler.min_filter),
                           getFilterMode(sampler.mip_map_mode), getWrapMode(sampler.wrap_mode_u),
                           getWrapMode(sampler.wrap_mode_v),    sampler.max_anisotropy};

    // Avoid duplicating samplers with same properties.
    const auto it = std::ranges::find_if(m_texture_samplers, [&data](const auto &tex_sampler) {
        return tex_sampler.isValid() && tex_sampler.getData() == data;
    });
    if (it == m_texture_samplers.end())
    {
        const auto index = static_cast<uint32_t>(m_texture_samplers.size());
        const std::string name = "Sampler_" + std::to_string(index);
        m_texture_samplers.emplace_back(device.createTextureSampler(name, data));
        return index;
    }

    return static_cast<uint32_t>(std::distance(m_texture_samplers.begin(), it));
}

uint32_t SceneData::addMesh(const scene::MeshHandle &handle, const scene::Mesh &mesh,
                            const std::unordered_map<scene::MaterialHandle, uint32_t> &material_indices,
                            std::unordered_map<scene::MeshHandle, uint32_t> &out_mesh_indices)
{
    if (out_mesh_indices.contains(handle))
    {
        return out_mesh_indices.at(handle);
    }

    const auto mesh_index = static_cast<uint32_t>(m_meshes.size());
    uint32_t material_index = 0;
    if (material_indices.contains(mesh.material))
    {
        material_index = material_indices.at(mesh.material);
    }
    m_meshes.emplace_back(MeshData{
        mesh.indices.format, static_cast<uint32_t>(mesh.indices.range.offset),
        static_cast<uint32_t>(mesh.indices.range.size), static_cast<uint32_t>(mesh.vertices.positions.offset),
        static_cast<uint32_t>(mesh.vertices.positions.size), static_cast<uint32_t>(mesh.vertices.normals.offset),
        static_cast<uint32_t>(mesh.vertices.normals.size), static_cast<uint32_t>(mesh.vertices.uvs.offset),
        static_cast<uint32_t>(mesh.vertices.uvs.size), static_cast<uint32_t>(mesh.vertices.colors.offset),
        static_cast<uint32_t>(mesh.vertices.colors.size), material_index});

    out_mesh_indices.insert(std::make_pair(handle, mesh_index));
    return mesh_index;
}


bool SceneData::init(const Device &device, const scene::Scene &scene)
{
    core::Logger::info(LOG_CHANNEL_VULKAN, "Transferring scene: " + scene.getName() + " resources to GPU.");
    m_fence = device.createFence("Fence_Data");
    m_encoder = device.createCommandEncoder("Encoder_Data", device.getTransferQueue());

    m_fence.reset();

    m_encoder.begin();

#pragma region TEXTURE DATA
    std::unordered_map<scene::ImageHandle, uint32_t> texture_indices{};
    std::vector<uint8_t> pixel_buffer{};
    scene.forEachImage([&](const scene::ImageHandle handle, const scene::Image &image) {
        const auto tex_index = static_cast<uint32_t>(m_textures.size());
        const auto tex_name = scene.getImageName(handle);
        uint32_t num_channels = image.getNumChannels();
        // Most GPUs do not support 3-channel sampled textures. So we use 4-channel pixel buffers.
        num_channels = num_channels == 3 ? 4 : num_channels;
        if (image.readPixelBuffer(pixel_buffer, scene::ImageChannelFormat::UNKNOWN, num_channels))
        {
            const auto texture = device.createTexture(
                m_encoder, tex_name, Size2D{image.getWidth(), image.getHeight()},
                getTextureFormatForImage(image.getChannelFormat(), num_channels, image.isColorSpaceSRGB()),
                TextureUsageFlags::SAMPLED | TextureUsageFlags::TRANSFER_DST, TextureLayout::SHADER_READ_ONLY_OPTIMAL,
                pixel_buffer.data());
            m_textures.emplace_back(texture);
            texture_indices.insert(std::make_pair(handle, tex_index));
        }
        else
        {
            core::Logger::warn(LOG_CHANNEL_VULKAN, "Skipping texture creation for image: " + tex_name);
        }
    });

    std::unordered_map<scene::TextureHandle, uint32_t> texture_data_indices{};
    scene.forEachTexture([&](const scene::TextureHandle handle, const scene::Texture &texture) {
        if (texture_indices.contains(texture.image))
        {
            const auto tex_data_index = static_cast<uint32_t>(m_texture_data.size());

            const uint32_t tex_index = texture_indices.at(texture.image);
            const uint32_t sampler_index = addTextureSampler(device, texture.sampler);
            m_texture_data.emplace_back(TextureData{texture.transform.offset, texture.transform.scale,
                                                    texture.transform.rotation, texture.tex_coord, tex_index,
                                                    sampler_index});

            texture_data_indices.insert(std::make_pair(handle, tex_data_index));
        }
    });

    if (!m_texture_data.empty())
    {
        const uint64_t size = m_texture_data.size() * sizeof(TextureData);
        m_buffer_texture_data = device.createBuffer(m_encoder, "Buffer_Texture_Data", size, m_texture_data.data(),
                                                    BufferUsageFlags::STORAGE_BUFFER);
    }

    const auto getTextureDataIndex = [&texture_data_indices](const scene::TextureHandle handle) -> uint32_t {
        if (texture_data_indices.contains(handle))
        {
            return texture_data_indices.at(handle);
        }
        return std::numeric_limits<uint32_t>::max();
    };
#pragma endregion

#pragma region MATERIAL DATA
    std::unordered_map<scene::MaterialHandle, uint32_t> material_indices{};
    scene.forEachMaterial([&](const scene::MaterialHandle handle, const scene::MaterialPBR &material) {
        const auto mat_index = static_cast<uint32_t>(m_materials.size());

        MaterialPBRData data{
            material.base_color,
            material.specular_color,
            material.specular_factor,
            material.emissive_color,
            material.emissive_strength,
            material.sheen_color,
            material.sheen_roughness_factor,
            material.metallic_factor,
            material.roughness_factor,
            material.anisotropy_strength,
            material.anisotropy_rotation,
            material.iridescence_factor,
            material.iridescence_ior,
            material.iridescence_thickness_min,
            material.iridescence_thickness_max,
            material.diffuse_transmission_color,
            material.diffuse_transmission_factor,
            material.volume_attenuation_color,
            material.volume_thickness_factor,
            material.volume_attenuation_distance,
            material.dispersion_factor,
            material.transmission_factor,
            material.ior,
            material.clearcoat_factor,
            material.clearcoat_roughness_factor,
            material.normal_scale,
            material.occlusion_strength,
            getTextureDataIndex(material.texture_base_color),
            getTextureDataIndex(material.texture_specular_color),
            getTextureDataIndex(material.texture_specular),
            getTextureDataIndex(material.texture_emissive),
            getTextureDataIndex(material.texture_sheen_color),
            getTextureDataIndex(material.texture_sheen_roughness),
            getTextureDataIndex(material.texture_metallic_roughness),
            getTextureDataIndex(material.texture_anisotropy),
            getTextureDataIndex(material.texture_iridescence),
            getTextureDataIndex(material.texture_iridescence_thickness),
            getTextureDataIndex(material.texture_diffuse_transmission_color),
            getTextureDataIndex(material.texture_diffuse_transmission),
            getTextureDataIndex(material.texture_volume_thickness),
            getTextureDataIndex(material.texture_transmission),
            getTextureDataIndex(material.texture_clearcoat),
            getTextureDataIndex(material.texture_clearcoat_roughness),
            getTextureDataIndex(material.texture_clearcoat_normal),
            getTextureDataIndex(material.texture_normal),
            getTextureDataIndex(material.texture_occlusion),
            material.alpha_mode,
            material.alpha_cutoff,
            material.double_sided,
        };

        m_materials.emplace_back(data);
        material_indices.insert(std::make_pair(handle, mat_index));
    });

    if (!m_materials.empty())
    {
        const uint64_t size = m_materials.size() * sizeof(MaterialPBRData);
        m_buffer_materials = device.createBuffer(m_encoder, "Buffer_Materials", size, m_materials.data(),
                                                 BufferUsageFlags::STORAGE_BUFFER);
    }
#pragma endregion

    const auto &i_buffers = scene.getIndexBuffer();
    if (!i_buffers.indices_8.empty())
    {
        const uint64_t size = i_buffers.indices_8.size();
        m_buffer_index_8 = device.createBuffer(m_encoder, "Buffer_Indices_UINT8", size, i_buffers.indices_8.data(),
                                               BufferUsageFlags::STORAGE_BUFFER | BufferUsageFlags::INDEX_BUFFER);
    }
    if (!i_buffers.indices_16.empty())
    {
        const uint64_t size = i_buffers.indices_16.size() * sizeof(uint16_t);
        m_buffer_index_16 = device.createBuffer(m_encoder, "Buffer_Indices_UINT16", size, i_buffers.indices_16.data(),
                                                BufferUsageFlags::STORAGE_BUFFER | BufferUsageFlags::INDEX_BUFFER);
    }
    if (!i_buffers.indices.empty())
    {
        const uint64_t size = i_buffers.indices.size() * sizeof(uint32_t);
        m_buffer_index_32 = device.createBuffer(m_encoder, "Buffer_Indices_UINT32", size, i_buffers.indices.data(),
                                                BufferUsageFlags::STORAGE_BUFFER | BufferUsageFlags::INDEX_BUFFER);
    }

    const auto &v_buffers = scene.getVertexBuffer();
    if (!v_buffers.positions.empty())
    {
        const uint64_t size = v_buffers.positions.size() * sizeof(scene::Vector3);
        m_buffer_position = device.createBuffer(m_encoder, "Buffer_Positions", size, v_buffers.positions.data(),
                                                BufferUsageFlags::STORAGE_BUFFER | BufferUsageFlags::VERTEX_BUFFER);
    }
    if (!v_buffers.normals.empty())
    {
        const uint64_t size = v_buffers.normals.size() * sizeof(scene::Vector3);
        m_buffer_normal = device.createBuffer(m_encoder, "Buffer_Normals", size, v_buffers.normals.data(),
                                              BufferUsageFlags::STORAGE_BUFFER | BufferUsageFlags::VERTEX_BUFFER);
    }
    if (!v_buffers.uvs.empty())
    {
        const uint64_t size = v_buffers.uvs.size() * sizeof(scene::Vector2);
        m_buffer_uv = device.createBuffer(m_encoder, "Buffer_UVs", size, v_buffers.uvs.data(),
                                          BufferUsageFlags::STORAGE_BUFFER | BufferUsageFlags::VERTEX_BUFFER);
    }
    if (!v_buffers.colors.empty())
    {
        const uint64_t size = v_buffers.colors.size() * sizeof(scene::Vector4);
        m_buffer_color = device.createBuffer(m_encoder, "Buffer_Colors", size, v_buffers.colors.data(),
                                             BufferUsageFlags::STORAGE_BUFFER | BufferUsageFlags::VERTEX_BUFFER);
    }

    std::unordered_map<scene::MeshHandle, uint32_t> mesh_indices;
    std::map<uint32_t, std::vector<InstanceData>> mesh_instances;
    scene.forEachRenderable([&](const scene::NodeHandle node_handle, const scene::MeshHandle mesh_handle,
                                const scene::HierarchyTransform &transform, const scene::Mesh &mesh) {
        const uint32_t mesh_index = addMesh(mesh_handle, mesh, material_indices, mesh_indices);
        if (!mesh_instances.contains(mesh_index))
        {
            mesh_instances.insert(std::make_pair(mesh_index, std::vector<InstanceData>{}));
        }
        auto &instances = mesh_instances.at(mesh_index);

        const auto transform_index = static_cast<uint32_t>(m_transforms.size());

        m_transforms.emplace_back(TransformData{transform.world.getWorldMatrix().getAsArray(),
                                                transform.world.getLocalMatrix().getAsArray()});
        instances.emplace_back(InstanceData{mesh_index, transform_index});
    });
    // Append each meshes' instances to the global instance list and keep track of its offset into the list.
    for (const auto &[mesh_index, instances] : mesh_instances)
    {
        const auto offset = static_cast<uint32_t>(m_mesh_instances.size());
        const auto count = static_cast<uint32_t>(instances.size());
        m_mesh_instances.insert(m_mesh_instances.end(), instances.begin(), instances.end());
        m_mesh_instance_map.insert(std::make_pair(mesh_index, MeshInstancesData{offset, count}));
    }

    if (!m_meshes.empty())
    {
        m_buffer_meshes = device.createBuffer(m_encoder, "Buffer_Meshes", m_meshes.size() * sizeof(MeshData),
                                              m_meshes.data(), BufferUsageFlags::STORAGE_BUFFER);
    }
    if (!m_mesh_instances.empty())
    {
        m_buffer_mesh_instances =
            device.createBuffer(m_encoder, "Buffer_Mesh_Instances", m_mesh_instances.size() * sizeof(InstanceData),
                                m_mesh_instances.data(), BufferUsageFlags::STORAGE_BUFFER);
    }

#pragma region ENVIRONMENT_LIGHT
    const auto &env_light = scene.getEnvironmentLight();
    m_environment_light.color = env_light.color;
    m_environment_light.intensity = env_light.intensity;
    m_environment_light.unit = env_light.unit;
    m_environment_light.rotation_matrix = env_light.rotation.getMatrix().getAsArray();
    m_environment_light.texture_index = getTextureDataIndex(env_light.texture);

    m_buffer_environment_light = device.createBuffer(m_encoder, "Buffer_Environment_Data", sizeof(EnvironmentLightData),
                                                     &m_environment_light, BufferUsageFlags::UNIFORM_BUFFER);
#pragma endregion

    // TODO: Upload light data to GPU.

    if (!m_transforms.empty())
    {
        m_buffer_transforms =
            device.createBuffer(m_encoder, "Buffer_Transforms", m_transforms.size() * sizeof(TransformData),
                                m_transforms.data(), BufferUsageFlags::STORAGE_BUFFER);
    }

    m_camera = CameraData{scene.getViewMatrix().getAsArray(),
                          (VULKAN_PROJECTION_INVERT_Y * scene.getProjectionMatrix()).getAsArray()};
    m_buffer_camera = device.createBuffer(m_encoder, "Buffer_Camera", sizeof(CameraData), &m_camera,
                                          BufferUsageFlags::UNIFORM_BUFFER);

    device.getTransferQueue().submit(m_encoder.finish(), m_fence);
    // TODO: Remove wait for fences for async scene data transfer.
    if (!m_fence.wait(FENCE_WAIT_TIMEOUT))
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to wait for scene resources being transferred to GPU");
    }
    if (device.tryReleaseTemporaryResources(m_fence))
    {
        core::Logger::info(LOG_CHANNEL_VULKAN, "Released temporary scene resources.");
    }
    return true;
}

void SceneData::destroy()
{
    // m_buffer_camera.destroy();
    //
    // m_buffer_transforms.destroy();
    m_transforms.clear();
    //
    // m_buffer_environment_light.destroy();
    //
    // m_buffer_mesh_instances.destroy();
    m_mesh_instance_map.clear();
    m_mesh_instances.clear();
    //
    // m_buffer_meshes.destroy();
    m_meshes.clear();
    //
    // m_buffer_color.destroy();
    // m_buffer_uv.destroy();
    // m_buffer_normal.destroy();
    // m_buffer_position.destroy();
    // m_buffer_index_32.destroy();
    // m_buffer_index_16.destroy();
    // m_buffer_index_8.destroy();
    //
    // m_buffer_materials.destroy();
    m_materials.clear();

    for (auto &s : m_texture_samplers)
    {
        s.destroy();
    }
    m_texture_samplers.clear();

    // m_buffer_texture_data.destroy();
    m_texture_data.clear();
    for (auto &t : m_textures)
    {
        t.destroy();
    }
    m_textures.clear();

    m_encoder.destroy();
    m_fence.destroy();
}

bool SceneData::updateCamera(const Device &device, const scene::Matrix4 &view_matrix,
                             const scene::Matrix4 &projection_matrix)
{
    if (!m_buffer_camera.isValid())
    {
        return false;
    }
    m_fence.reset();
    m_encoder.begin();
    m_camera = CameraData{view_matrix.getAsArray(), (VULKAN_PROJECTION_INVERT_Y * projection_matrix).getAsArray()};

    device.getBuffer(m_buffer_camera)->write(&m_encoder, sizeof(CameraData), &m_camera);
    // m_buffer_camera.update(m_encoder, sizeof(CameraData), &m_camera);
    device.getTransferQueue().submit(m_encoder.finish(), m_fence);
    // TODO: Remove wait for fences for async scene data transfer.
    if (!m_fence.wait(FENCE_WAIT_TIMEOUT))
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to wait for scene resources being transferred to GPU");
    }
    if (device.tryReleaseTemporaryResources(m_fence))
    {
        core::Logger::info(LOG_CHANNEL_VULKAN, "Released temporary scene resources.");
    }
    return true;
}
} // namespace kirana::renderer