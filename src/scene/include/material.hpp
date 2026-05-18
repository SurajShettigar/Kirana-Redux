// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_MATERIAL_HPP
#define KIRANA_SCENE_MATERIAL_HPP

#include "common.hpp"

#include <resource.hpp>

namespace kirana::scene
{
enum class AlphaMode : uint32_t
{
    OPAQUE = 0u,
    MASK = 1u,
    BLEND = 2u,
};

struct MaterialPBR final : core::IResource
{
    AlphaMode alpha_mode{AlphaMode::OPAQUE};
    float alpha_cutoff{0.5f};
    bool double_sided{false};

    std::array<float, 4> base_color{1.0f, 1.0f, 1.0f, 1.0f};
    std::array<float, 3> specular_color{1.0f, 1.0f, 1.0f};
    float specular_factor{0.0f};
    std::array<float, 3> emissive_color{0.0f, 0.0f, 0.0f};
    float emissive_strength{1.0f};

    float metallic_factor{1.0f};
    float roughness_factor{1.0f};
    float anisotropy_strength{0.0f};
    float anisotropy_rotation{0.0f};
    float normal_scale{1.0f};
    float occlusion_strength{1.0f};

    std::array<float, 3> diffuse_transmission_color{0.0f, 0.0f, 0.0f};
    float diffuse_transmission_factor{0.0f};
    float transmission_factor{0.0f};
    float ior{1.5f};
    float dispersion_factor{0.0f};
    std::array<float, 3> volume_attenuation_color{1.0f, 1.0f, 1.0f};
    float volume_thickness_factor{0.0f};
    float volume_attenuation_distance{std::numeric_limits<float>::infinity()};

    float iridescence_factor{0.0f};
    float iridescence_ior{1.3f};
    float iridescence_thickness_min{100.0f};
    float iridescence_thickness_max{400.0f};

    float clearcoat_factor{0.0f};
    float clearcoat_roughness_factor{0.0f};
    std::array<float, 3> sheen_color{0.0f};
    float sheen_roughness_factor{0.0f};

    TextureHandle texture_base_color{};
    /// R - Undefined, G - Roughness, B - Metallic
    TextureHandle texture_metallic_roughness{};
    TextureHandle texture_normal{};
    TextureHandle texture_occlusion{};
    TextureHandle texture_emissive{};
    /// RG - Tangent, Bitangent rotation direction (Convert [0, 1] to -[1, 1] range). B - Anisotropy strength.
    TextureHandle texture_anisotropy{};
    /// R - Clearcoat strength.
    TextureHandle texture_clearcoat{};
    /// G - Clearcoat roughness.
    TextureHandle texture_clearcoat_roughness{};
    TextureHandle texture_clearcoat_normal{};
    /// A - Diffuse transmission factor.
    TextureHandle texture_diffuse_transmission{};
    TextureHandle texture_diffuse_transmission_color{};
    /// R - Iridescence factor.
    TextureHandle texture_iridescence{};
    /// G - Iridescence thickness. Value is used to lerp from min-max thickness values.
    TextureHandle texture_iridescence_thickness{};
    TextureHandle texture_sheen_color{};
    /// A - Sheen roughness factor.
    TextureHandle texture_sheen_roughness{};
    TextureHandle texture_specular_color{};
    /// A - Specular factor.
    TextureHandle texture_specular{};
    TextureHandle texture_transmission{};
    /// G - Volume thickness factor.
    TextureHandle texture_volume_thickness{};

    MaterialPBR() : IResource{"Material_PBR"}
    {
    }
    explicit MaterialPBR(const std::string &name) : IResource{name}
    {
    }
    explicit MaterialPBR(
        const std::string &name, const AlphaMode alpha_mode = AlphaMode::OPAQUE, const float alpha_cutoff = 0.5f,
        const bool double_sided = false, const std::array<float, 4> &base_color = {1.0f, 1.0f, 1.0f, 1.0f},
        const std::array<float, 3> &specular_color = {1.0f, 1.0f, 1.0f}, const float specular_factor = 0.0f,
        const std::array<float, 3> &emissive_color = {0.0f, 0.0f, 0.0f}, const float emissive_strength = 1.0f,
        const float metallic_factor = 1.0f, const float roughness_factor = 1.0f, const float anisotropy_strength = 0.0f,
        const float anisotropy_rotation = 0.0f, const float normal_scale = 1.0f, const float occlusion_strength = 1.0f,
        const std::array<float, 3> &diffuse_transmission_color = {0.0f, 0.0f, 0.0f},
        const float diffuse_transmission_factor = 0.0f, const float transmission_factor = 0.0f, const float ior = 1.5f,
        const float dispersion_factor = 0.0f, const std::array<float, 3> &volume_attenuation_color = {1.0f, 1.0f, 1.0f},
        const float volume_thickness_factor = 0.0f,
        const float volume_attenuation_distance = std::numeric_limits<float>::infinity(),
        const float iridescence_factor = 0.0f, const float iridescence_ior = 1.3f,
        const float iridescence_thickness_min = 100.0f, const float iridescence_thickness_max = 400.0f,
        const float clearcoat_factor = 0.0f, const float clearcoat_roughness_factor = 0.0f,
        const std::array<float, 3> &sheen_color = {0.0f, 0.0f, 0.0f}, const float sheen_roughness_factor = 0.0f,
        const TextureHandle texture_base_color = {}, const TextureHandle texture_metallic_roughness = {},
        const TextureHandle texture_normal = {}, const TextureHandle texture_occlusion = {},
        const TextureHandle texture_emissive = {}, const TextureHandle texture_anisotropy = {},
        const TextureHandle texture_clearcoat = {}, const TextureHandle texture_clearcoat_roughness = {},
        const TextureHandle texture_clearcoat_normal = {}, const TextureHandle texture_diffuse_transmission = {},
        const TextureHandle texture_diffuse_transmission_color = {}, const TextureHandle texture_iridescence = {},
        const TextureHandle texture_iridescence_thickness = {}, const TextureHandle texture_sheen_color = {},
        const TextureHandle texture_sheen_roughness = {}, const TextureHandle texture_specular_color = {},
        const TextureHandle texture_specular = {}, const TextureHandle texture_transmission = {},
        const TextureHandle texture_volume_thickness = {})
        : IResource{name}, alpha_mode{alpha_mode}, alpha_cutoff{alpha_cutoff}, double_sided{double_sided},
          base_color{base_color}, specular_color{specular_color}, specular_factor{specular_factor},
          emissive_color{emissive_color}, emissive_strength{emissive_strength}, metallic_factor{metallic_factor},
          roughness_factor{roughness_factor}, anisotropy_strength{anisotropy_strength},
          anisotropy_rotation{anisotropy_rotation}, normal_scale{normal_scale}, occlusion_strength{occlusion_strength},
          diffuse_transmission_color{diffuse_transmission_color},
          diffuse_transmission_factor{diffuse_transmission_factor}, transmission_factor{transmission_factor}, ior{ior},
          dispersion_factor{dispersion_factor}, volume_attenuation_color{volume_attenuation_color},
          volume_thickness_factor{volume_thickness_factor}, volume_attenuation_distance{volume_attenuation_distance},
          iridescence_factor{iridescence_factor}, iridescence_ior{iridescence_ior},
          iridescence_thickness_min{iridescence_thickness_min}, iridescence_thickness_max{iridescence_thickness_max},
          clearcoat_factor{clearcoat_factor}, clearcoat_roughness_factor{clearcoat_roughness_factor},
          sheen_color{sheen_color}, sheen_roughness_factor{sheen_roughness_factor},
          texture_base_color{texture_base_color}, texture_metallic_roughness{texture_metallic_roughness},
          texture_normal{texture_normal}, texture_occlusion{texture_occlusion}, texture_emissive{texture_emissive},
          texture_anisotropy{texture_anisotropy}, texture_clearcoat{texture_clearcoat},
          texture_clearcoat_roughness{texture_clearcoat_roughness}, texture_clearcoat_normal{texture_clearcoat_normal},
          texture_diffuse_transmission{texture_diffuse_transmission},
          texture_diffuse_transmission_color{texture_diffuse_transmission_color},
          texture_iridescence{texture_iridescence}, texture_iridescence_thickness{texture_iridescence_thickness},
          texture_sheen_color{texture_sheen_color}, texture_sheen_roughness{texture_sheen_roughness},
          texture_specular_color{texture_specular_color}, texture_specular{texture_specular},
          texture_transmission{texture_transmission}, texture_volume_thickness{texture_volume_thickness}
    {
    }

  protected:
    bool doLoad() override
    {
        return true;
    }
    void doUnload() override
    {
    }
};
} // namespace kirana::scene

#endif // KIRANA_SCENE_MATERIAL_HPP