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

    std::array<float, 4> base_color{1.0f};
    std::array<float, 3> specular_color{1.0f};
    float specular_factor{0.0f};
    std::array<float, 3> emissive_color{0.0f};
    float emissive_strength{1.0f};

    float metallic_factor{1.0f};
    float roughness_factor{1.0f};
    float anisotropy_strength{0.0f};
    float anisotropy_rotation{0.0f};
    float normal_scale{1.0f};
    float occlusion_strength{1.0f};

    std::array<float, 3> diffuse_transmission_color{0.0f};
    float diffuse_transmission_factor{0.0f};
    float transmission_factor{0.0f};
    float ior{1.5f};
    float dispersion_factor{0.0f};
    std::array<float, 3> volume_attenuation_color{1.0f};
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

    MaterialPBR() = default;

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