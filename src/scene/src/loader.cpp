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
        const auto it = std::ranges::find_if(EXTENSIONS_GLTF, [&](const auto &e) { return e == out_extension; });
        if (it != EXTENSIONS_GLTF.end())
        {
            return SceneFileFormat::GLTF;
        }
    }
    return SceneFileFormat::UNKNOWN;
}

inline std::unordered_map<uint32_t, ImageHandle> loadGLTFImages(const GLTFLoader &loader, Scene *out_scene)
{
    const auto &doc = loader.getDocument();

    std::unordered_map<uint32_t, ImageHandle> images{};
    for (uint32_t i_index = 0; i_index < doc.images.size(); ++i_index)
    {
        const auto &gltf_image = doc.images[i_index];
        if (!gltf_image.isValid())
        {
            continue;
        }

        std::string image_name{gltf_image.name.value_or("")};
        ImageHandle img_handle{};
        // Image can either be a path, data uri or an embedded buffer. We call the appropriate function to add the
        // image based on what's returned by the GLTF loader.
        if (const auto &img_path_buffer = loader.getImage(i_index); img_path_buffer.index() == 0)
        {
            img_handle = out_scene->addImage(image_name, std::get<std::string>(img_path_buffer));
        }
        else
        {
            image_name = image_name.empty() ? "Image_Embedded_" + std::to_string(i_index) : image_name;
            std::string image_file = image_name;
            if (const auto mime_type = gltf_image.mime_type.value_or(GLTFImageMimeType::PNG);
                mime_type == GLTFImageMimeType::PNG)
            {
                image_file += ".png";
            }
            else if (mime_type == GLTFImageMimeType::JPEG)
            {
                image_file += ".jpg";
            }
            core::Filepath image_path = core::Filepath(loader.getFileRootPath()) / core::Filepath(image_file);
            img_handle =
                out_scene->addImage(image_name, image_path.string(), std::get<std::vector<uint8_t>>(img_path_buffer));
        }
        images.insert_or_assign(i_index, img_handle);
    }
    return images;
}

inline std::vector<TextureSampler> loadGLTFTextureSamplers(const GLTFDocument &doc)
{
    std::vector<TextureSampler> samplers{};
    samplers.reserve(doc.samplers.size());
    const auto get_filter_mode = [](const GLTFFilter &filter) {
        switch (filter)
        {
        case GLTFFilter::NEAREST:
        case GLTFFilter::NEAREST_MIPMAP_LINEAR:
        case GLTFFilter::NEAREST_MIPMAP_NEAREST:
            return TextureFilterMode::NEAREST;
        case GLTFFilter::LINEAR:
        case GLTFFilter::LINEAR_MIPMAP_LINEAR:
        case GLTFFilter::LINEAR_MIPMAP_NEAREST:
            return TextureFilterMode::LINEAR;
        default:
            return TextureFilterMode::NEAREST;
        }
    };
    const auto get_mip_map_mode = [](const GLTFFilter &filter) {
        switch (filter)
        {
        case GLTFFilter::NEAREST_MIPMAP_NEAREST:
        case GLTFFilter::LINEAR_MIPMAP_NEAREST:
            return TextureFilterMode::NEAREST;
        case GLTFFilter::NEAREST_MIPMAP_LINEAR:
        case GLTFFilter::LINEAR_MIPMAP_LINEAR:
            return TextureFilterMode::LINEAR;
        default:
            return TextureFilterMode::NEAREST;
        }
    };
    const auto get_wrap_mode = [](const GLTFWrapMode &wrap) {
        switch (wrap)
        {
        case GLTFWrapMode::REPEAT:
            return TextureWrapMode::REPEAT;
        case GLTFWrapMode::CLAMP_TO_EDGE:
            return TextureWrapMode::CLAMP_TO_EDGE;
        case GLTFWrapMode::MIRRORED_REPEAT:
            return TextureWrapMode::MIRRORED_REPEAT;
        default:
            return TextureWrapMode::REPEAT;
        }
    };
    for (const auto &gltf_sampler : doc.samplers)
    {
        samplers.emplace_back(get_filter_mode(gltf_sampler.mag_filter.value_or(GLTFFilter::NEAREST)),
                              get_filter_mode(gltf_sampler.min_filter.value_or(GLTFFilter::NEAREST)),
                              get_mip_map_mode(gltf_sampler.min_filter.value_or(GLTFFilter::NEAREST)),
                              get_wrap_mode(gltf_sampler.wrap_s), get_wrap_mode(gltf_sampler.wrap_t));
    }
    return samplers;
}

inline std::unordered_map<uint32_t, TextureHandle> loadGLTFTextures(
    const GLTFDocument &doc, const std::unordered_map<uint32_t, ImageHandle> &images,
    const std::vector<TextureSampler> &samplers, Scene *out_scene)
{
    std::unordered_map<uint32_t, TextureHandle> textures{};
    for (uint32_t t_index = 0; t_index < doc.textures.size(); ++t_index)
    {
        const auto &gltf_texture = doc.textures[t_index];
        std::string tex_name{gltf_texture.name.value_or("")};

        const auto image = gltf_texture.source ? images.at(gltf_texture.source.value()) : ImageHandle{};
        const auto sampler = gltf_texture.sampler ? samplers.at(gltf_texture.sampler.value()) : TextureSampler{};

        textures.insert_or_assign(t_index, out_scene->addTexture(tex_name, image, sampler));
    }
    return textures;
}

inline std::unordered_map<uint32_t, MaterialHandle> loadGLTFMaterials(
    const GLTFDocument &doc, const std::unordered_map<uint32_t, TextureHandle> &textures, Scene *out_scene)
{
    std::unordered_map<uint32_t, MaterialHandle> materials{};

    const auto getAlphaMode = [](const GLTFAlphaMode &alpha_mode) {
        switch (alpha_mode)
        {
        case GLTFAlphaMode::OPAQUE:
            return AlphaMode::OPAQUE;
        case GLTFAlphaMode::MASK:
            return AlphaMode::MASK;
        case GLTFAlphaMode::BLEND:
            return AlphaMode::BLEND;
        default:
            return AlphaMode::OPAQUE;
        }
    };
    const auto getTextureHandle =
        [&](const std::variant<std::optional<GLTFTextureInfo>, std::optional<GLTFTextureInfoNormal>,
                               std::optional<GLTFTextureInfoOcclusion>> &tex_info) {
            if (tex_info.index() == 0)
            {
                const auto &info = std::get<std::optional<GLTFTextureInfo>>(tex_info);
                return info ? textures.at(info.value().index) : TextureHandle{};
            }
            if (tex_info.index() == 1)
            {
                const auto &info = std::get<std::optional<GLTFTextureInfoNormal>>(tex_info);
                return info ? textures.at(info.value().index) : TextureHandle{};
            }
            const auto &info = std::get<std::optional<GLTFTextureInfoOcclusion>>(tex_info);
            return info ? textures.at(info.value().index) : TextureHandle{};
        };
    const auto updateTexture =
        [&](const std::variant<std::optional<GLTFTextureInfo>, std::optional<GLTFTextureInfoNormal>,
                               std::optional<GLTFTextureInfoOcclusion>> &tex_info,
            const TextureHandle &handle, const bool is_srgb = false) {
            uint32_t tex_coord = 0u;
            GLTFTextureTransform transform{};
            if (tex_info.index() == 0)
            {
                if (const auto &info = std::get<std::optional<GLTFTextureInfo>>(tex_info); info)
                {
                    tex_coord = info.value().tex_coord;
                    if (info.value().transform)
                    {
                        transform = info.value().transform.value();
                    }
                }
            }
            else if (tex_info.index() == 1)
            {
                if (const auto &info = std::get<std::optional<GLTFTextureInfoNormal>>(tex_info); info)
                {
                    tex_coord = info.value().tex_coord;
                    if (info.value().transform)
                    {
                        transform = info.value().transform.value();
                    }
                }
            }
            else
            {
                if (const auto &info = std::get<std::optional<GLTFTextureInfoOcclusion>>(tex_info); info)
                {
                    tex_coord = info.value().tex_coord;
                    if (info.value().transform)
                    {
                        transform = info.value().transform.value();
                    }
                }
            }
            if (const auto texture = out_scene->getTexture(handle); texture)
            {
                if (const auto image = out_scene->getImage(texture->image))
                {
                    image->setColorSpace(is_srgb);
                }
                texture->tex_coord = tex_coord;
                texture->transform = TextureTransform{transform.offset, transform.scale, transform.rotation};
            }
        };

    for (uint32_t m_index = 0; m_index < doc.materials.size(); ++m_index)
    {
        const auto &gltf_material = doc.materials[m_index];
        std::string mat_name{gltf_material.name.value_or("")};

        MaterialPBR material{};
        material.alpha_mode = getAlphaMode(gltf_material.alpha_mode);
        material.alpha_cutoff = gltf_material.alpha_cutoff;
        material.double_sided = gltf_material.double_sided;

        if (gltf_material.pbr_metallic_roughness)
        {
            const auto &pbr = gltf_material.pbr_metallic_roughness.value();
            material.base_color = pbr.base_color_factor;
            material.metallic_factor = pbr.metallic_factor;
            material.roughness_factor = pbr.roughness_factor;
            if (const auto tex_handle = getTextureHandle(pbr.base_color_texture); tex_handle.isValid())
            {
                material.texture_base_color = tex_handle;
                updateTexture(pbr.base_color_texture, tex_handle, true);
            }
            if (const auto tex_handle = getTextureHandle(pbr.metallic_roughness_texture); tex_handle.isValid())
            {
                material.texture_metallic_roughness = tex_handle;
                updateTexture(pbr.metallic_roughness_texture, tex_handle);
            }
        }

        if (const auto tex_handle = getTextureHandle(gltf_material.normal_texture); tex_handle.isValid())
        {
            material.normal_scale = gltf_material.normal_texture.value().scale;
            material.texture_normal = tex_handle;
            updateTexture(gltf_material.normal_texture, tex_handle);
        }
        if (const auto tex_handle = getTextureHandle(gltf_material.occlusion_texture); tex_handle.isValid())
        {
            material.occlusion_strength = gltf_material.occlusion_texture.value().strength;
            material.texture_occlusion = tex_handle;
            updateTexture(gltf_material.occlusion_texture, tex_handle);
        }

        material.emissive_color = gltf_material.emissive_factor;
        if (gltf_material.emissive_strength)
        {
            material.emissive_strength = gltf_material.emissive_strength.value().strength;
        }
        if (const auto tex_handle = getTextureHandle(gltf_material.emissive_texture); tex_handle.isValid())
        {
            material.texture_emissive = tex_handle;
            updateTexture(gltf_material.emissive_texture, tex_handle, true);
        }

        if (gltf_material.specular)
        {
            const auto &spec = gltf_material.specular.value();
            material.specular_color = spec.color_factor;
            material.specular_factor = spec.factor;
            if (const auto tex_handle = getTextureHandle(spec.texture); tex_handle.isValid())
            {
                material.texture_specular = tex_handle;
                updateTexture(spec.texture, tex_handle);
            }
            if (const auto tex_handle = getTextureHandle(spec.color_texture); tex_handle.isValid())
            {
                material.texture_specular_color = tex_handle;
                updateTexture(spec.color_texture, tex_handle, true);
            }
        }

        if (gltf_material.anisotropy)
        {
            const auto &aniso = gltf_material.anisotropy.value();
            material.anisotropy_strength = aniso.strength;
            material.anisotropy_rotation = aniso.rotation;
            if (const auto tex_handle = getTextureHandle(aniso.texture); tex_handle.isValid())
            {
                material.texture_anisotropy = tex_handle;
                updateTexture(aniso.texture, tex_handle);
            }
        }

        if (gltf_material.diffuse_transmission)
        {
            const auto &diff = gltf_material.diffuse_transmission.value();
            material.diffuse_transmission_color = diff.color_factor;
            material.diffuse_transmission_factor = diff.factor;
            if (const auto tex_handle = getTextureHandle(diff.texture); tex_handle.isValid())
            {
                material.texture_diffuse_transmission = tex_handle;
                updateTexture(diff.texture, tex_handle);
            }
            if (const auto tex_handle = getTextureHandle(diff.color_texture); tex_handle.isValid())
            {
                material.texture_diffuse_transmission_color = tex_handle;
                updateTexture(diff.color_texture, tex_handle, true);
            }
        }
        if (gltf_material.transmission)
        {
            const auto &trans = gltf_material.transmission.value();
            material.transmission_factor = trans.factor;
            if (const auto tex_handle = getTextureHandle(trans.texture); tex_handle.isValid())
            {
                material.texture_transmission = tex_handle;
                updateTexture(trans.texture, tex_handle);
            }
        }
        if (gltf_material.ior)
        {
            material.ior = gltf_material.ior.value().ior;
        }
        if (gltf_material.dispersion)
        {
            material.dispersion_factor = gltf_material.dispersion.value().dispersion;
        }
        if (gltf_material.volume)
        {
            const auto &vol = gltf_material.volume.value();
            material.volume_attenuation_color = vol.attenuation_color;
            material.volume_thickness_factor = vol.thickness_factor;
            material.volume_attenuation_distance = vol.attenuation_distance;
            if (const auto tex_handle = getTextureHandle(vol.thickness_texture); tex_handle.isValid())
            {
                material.texture_volume_thickness = tex_handle;
                updateTexture(vol.thickness_texture, tex_handle);
            }
        }

        if (gltf_material.iridescence)
        {
            const auto &ir = gltf_material.iridescence.value();
            material.iridescence_factor = ir.factor;
            material.ior = ir.ior;
            material.iridescence_thickness_min = ir.thickness_min;
            material.iridescence_thickness_max = ir.thickness_max;
            if (const auto tex_handle = getTextureHandle(ir.texture); tex_handle.isValid())
            {
                material.texture_iridescence = tex_handle;
                updateTexture(ir.texture, tex_handle);
            }
            if (const auto tex_handle = getTextureHandle(ir.thickness_texture); tex_handle.isValid())
            {
                material.texture_iridescence_thickness = tex_handle;
                updateTexture(ir.thickness_texture, tex_handle);
            }
        }

        if (gltf_material.clearcoat)
        {
            const auto &cc = gltf_material.clearcoat.value();
            material.clearcoat_factor = cc.factor;
            material.clearcoat_roughness_factor = cc.roughness_factor;
            if (const auto tex_handle = getTextureHandle(cc.texture); tex_handle.isValid())
            {
                material.texture_clearcoat = tex_handle;
                updateTexture(cc.texture, tex_handle);
            }
            if (const auto tex_handle = getTextureHandle(cc.roughness_texture); tex_handle.isValid())
            {
                material.texture_clearcoat_roughness = tex_handle;
                updateTexture(cc.roughness_texture, tex_handle);
            }
            if (const auto tex_handle = getTextureHandle(cc.normal_texture); tex_handle.isValid())
            {
                material.texture_clearcoat_normal = tex_handle;
                updateTexture(cc.normal_texture, tex_handle);
            }
        }

        if (gltf_material.sheen)
        {
            const auto &sheen = gltf_material.sheen.value();
            material.sheen_color = sheen.color_factor;
            material.sheen_roughness_factor = sheen.roughness_factor;
            if (const auto tex_handle = getTextureHandle(sheen.color_texture); tex_handle.isValid())
            {
                material.texture_sheen_color = tex_handle;
                updateTexture(sheen.color_texture, tex_handle, true);
            }
            if (const auto tex_handle = getTextureHandle(sheen.roughness_texture); tex_handle.isValid())
            {
                material.texture_sheen_roughness = tex_handle;
                updateTexture(sheen.roughness_texture, tex_handle);
            }
        }

        materials.insert_or_assign(m_index, out_scene->addMaterial(mat_name, material));
    }
    return materials;
}

inline std::unordered_map<uint32_t, CameraHandle> loadGLTFCameras(const GLTFDocument &doc, Scene *out_scene)
{
    std::unordered_map<uint32_t, CameraHandle> cameras{};
    for (uint32_t c_index = 0; c_index < doc.cameras.size(); ++c_index)
    {
        const auto &cam = doc.cameras[c_index];
        if (!cam.isValid())
        {
            continue;
        }
        std::string cam_name{cam.name.value_or("")};
        if (cam.type == GLTFCameraType::PERSPECTIVE)
        {
            const auto &p_cam = cam.perspective.value();
            cameras.insert_or_assign(
                c_index, out_scene->addPerspectiveCamera(
                             cam_name, {p_cam.z_near, p_cam.z_far.value_or(Camera::INFINITE_FAR_CLIPPING_PLANE)},
                             ganita::degree(p_cam.fov_y), p_cam.aspect_ratio.value_or(1.0f)));
        }
        else if (cam.type == GLTFCameraType::ORTHOGRAPHIC)
        {
            const auto &o_cam = cam.orthographic.value();
            cameras.insert_or_assign(c_index,
                                     out_scene->addOrthographicCamera(cam_name, {o_cam.z_near, o_cam.z_far},
                                                                      {o_cam.magnification_x, o_cam.magnification_y}));
        }
    }
    return cameras;
}

inline std::unordered_map<uint32_t, PunctualLightHandle> loadGLTFLights(const GLTFDocument &doc, Scene *out_scene)
{
    std::unordered_map<uint32_t, PunctualLightHandle> lights{};
    for (uint32_t l_index = 0; l_index < doc.lights.size(); ++l_index)
    {
        const auto &gltf_light = doc.lights[l_index];
        std::string light_name{gltf_light.name.value_or("")};
        if (gltf_light.type == GLTFPunctualLightType::DIRECTIONAL)
        {
            lights.insert_or_assign(l_index,
                                    out_scene->addDirectionalLight(light_name, gltf_light.color, gltf_light.intensity));
        }
        else if (gltf_light.type == GLTFPunctualLightType::POINT)
        {
            lights.insert_or_assign(l_index,
                                    out_scene->addPointLight(light_name, gltf_light.color, gltf_light.intensity,
                                                             gltf_light.range.value_or(-1.0f)));
        }
        else if (gltf_light.type == GLTFPunctualLightType::SPOT)
        {
            if (const auto &spot = gltf_light.spot; spot)
            {

                lights.insert_or_assign(
                    l_index, out_scene->addSpotLight(light_name, gltf_light.color, gltf_light.intensity,
                                                     gltf_light.range.value_or(-1.0f), spot.value().cone_angle_inner,
                                                     spot.value().cone_angle_outer));
            }
            else
            {
                lights.insert_or_assign(l_index,
                                        out_scene->addSpotLight(light_name, gltf_light.color, gltf_light.intensity,
                                                                gltf_light.range.value_or(-1.0f)));
            }
        }
    }
    return lights;
}

inline std::unordered_map<uint32_t, std::vector<MeshHandle>> loadGLTFMeshes(
    GLTFLoader &loader, const std::unordered_map<uint32_t, MaterialHandle> &materials, Scene *out_scene)
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
                                        "Unsupported index buffer component type: " +
                                            std::to_string(static_cast<int>(accessor.component_type)));
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
                    core::Logger::error(LOG_CHANNEL_SCENE, "Unsupported position attribute accessor type: " +
                                                               std::to_string(static_cast<int>(pos_accessor.type)));
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
                    core::Logger::error(LOG_CHANNEL_SCENE, "Unsupported normal attribute accessor type: " +
                                                               std::to_string(static_cast<int>(normal_accessor.type)));
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
                    core::Logger::error(LOG_CHANNEL_SCENE, "Unsupported texture coordinate attribute accessor type: " +
                                                               std::to_string(static_cast<int>(uv_accessor.type)));
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
                    core::Logger::error(LOG_CHANNEL_SCENE, "Unsupported color attribute accessor type: " +
                                                               std::to_string(static_cast<int>(color_accessor.type)));
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
            const auto material = prim.material ? materials.at(prim.material.value()) : MaterialHandle{};
            mesh_handles.emplace_back(out_scene->addMesh(mesh_name, index_buffer, vertex_buffer, material));
        }
        meshes.insert_or_assign(m_index, std::move(mesh_handles));
    }
    return meshes;
}

void loadGLTFNodes(const GLTFDocument &doc, const std::vector<uint32_t> &node_indices,
                   const std::unordered_map<uint32_t, CameraHandle> &cameras,
                   const std::unordered_map<uint32_t, PunctualLightHandle> &lights,
                   const std::unordered_map<uint32_t, std::vector<MeshHandle>> &meshes,
                   const std::optional<NodeHandle> &parent_node, Scene *out_scene)
{
    for (const auto &node_index : node_indices)
    {
        const auto &node = doc.nodes.at(node_index);

        std::optional<NodeHandle> current_parent = std::nullopt;
        std::string node_name = node.name.value_or("");
        auto transform = node.matrix ? Transform{Matrix4{node.matrix.value()}.transpose()}
                                     : Transform{node.translation.value_or({0.0f, 0.0f, 0.0f}),
                                                 node.rotation.value_or({0.0f, 0.0f, 0.0f, 1.0f}),
                                                 node.scale.value_or({1.0f, 1.0f, 1.0f})};
        if (node.camera && cameras.contains(node.camera.value()))
        {
            const auto &camera_handle = cameras.at(node.camera.value());
            node_name = node_name.empty() ? out_scene->getCameraName(camera_handle) : node_name;
            current_parent = out_scene->addNode(node_name, NodeFlags::NONE, transform, camera_handle, parent_node);
        }
        else if (node.light && lights.contains(node.light.value()))
        {
            const auto &light_handle = lights.at(node.light.value());
            node_name = node_name.empty() ? out_scene->getPunctualLightName(light_handle) : node_name;
            current_parent = out_scene->addNode(node_name, NodeFlags::NONE, transform, light_handle, parent_node);
        }
        else if (node.mesh && meshes.contains(node.mesh.value()))
        {
            const auto &mesh_handles = meshes.at(node.mesh.value());
            // Group meshes with multiple mesh primitives under a single node.
            const bool is_group = mesh_handles.size() > 1;
            std::optional<NodeHandle> group_handle = parent_node;
            if (is_group)
            {
                group_handle = out_scene->addNode(node_name, NodeFlags::NONE, transform, std::nullopt, parent_node);
                // The children of the current node will have this grouped node as parent.
                current_parent = group_handle;
                transform = Transform{};
            }
            for (const auto &mesh_handle : mesh_handles)
            {
                node_name = node_name.empty() ? out_scene->getMeshName(mesh_handle) : node_name;
                const auto handle =
                    out_scene->addNode(node_name, NodeFlags::NONE, transform, mesh_handle, group_handle);
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
            const auto handle = out_scene->addNode(node_name, NodeFlags::NONE, transform, std::nullopt, parent_node);
            current_parent = handle;
        }
        if (!node.children.empty())
        {
            loadGLTFNodes(doc, node.children, cameras, lights, meshes, current_parent, out_scene);
        }
    }
}

inline bool loadGLTF(const SceneFileInfo &info, Scene *out_scene)
{
    auto loader = GLTFLoader(info.path, true);
    if (!loader.isValid())
    {
        return false;
    }
    const auto &doc = loader.getDocument();

    const auto &images = loadGLTFImages(loader, out_scene);
    const auto &samplers = loadGLTFTextureSamplers(doc);
    const auto &textures = loadGLTFTextures(doc, images, samplers, out_scene);
    const auto &materials = loadGLTFMaterials(doc, textures, out_scene);
    const auto &cameras = loadGLTFCameras(doc, out_scene);
    const auto &lights = loadGLTFLights(doc, out_scene);
    const auto &meshes = loadGLTFMeshes(loader, materials, out_scene);
    if (!doc.scenes.empty())
    {
        // TODO: Add option to load multiple GLTF scenes.
        const auto &[nodes, name] = doc.scenes.front();
        if (const auto scene_name = name.value_or(""); !scene_name.empty())
        {
            out_scene->setName(scene_name);
        }
        loadGLTFNodes(doc, nodes, cameras, lights, meshes, std::nullopt, out_scene);
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
} // namespace kirana::scene