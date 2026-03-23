// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_LIGHT_HPP
#define KIRANA_SCENE_LIGHT_HPP

#include <resource_manager.hpp>

namespace kirana::scene
{
enum struct PunctualLightType : uint32_t
{
    DIRECTIONAL = 0u,
    POINT = 1u,
    SPOT = 2u,
};

enum struct LightUnit : uint32_t
{
    /// Luminous FLux (lm) Radiometric Unit => Radiant Flux (watt (W))
    LUMEN = 0u,
    /// Luminous Intensity (lm/sr = cd) Radiometric Unit => Intensity (W/sr)
    CANDELA = 1u,
    /// Illuminance (lm/m² = lx) Radiometric Unit => Irradiance (W/m²)
    LUX = 2u,
    /// Luminance (lm/m²sr = cd/m²) Radiometric Unit => Radiance (W/m²sr)
    NIT = 2u,
};

static const std::unordered_map DEFAULT_LIGHT_UNITS{std::make_pair(PunctualLightType::DIRECTIONAL, LightUnit::LUX),
                                                    std::make_pair(PunctualLightType::POINT, LightUnit::CANDELA),
                                                    std::make_pair(PunctualLightType::SPOT, LightUnit::CANDELA)};

struct PunctualLight final : core::IResource
{
    PunctualLightType type{PunctualLightType::DIRECTIONAL};
    LightUnit unit{LightUnit::LUX};
    std::array<float, 3> color{1.0f, 1.0f, 1.0f};
    float intensity{1.0f};
    float range{-1.0f};
    float spot_cone_angle_inner{0.0f};           // radians
    float spot_cone_angle_outer{0.78539816339f}; // radians

    PunctualLight() = default;

    explicit PunctualLight(const PunctualLightType type, const LightUnit unit, const std::array<float, 3> &color,
                           const float intensity, const float range = -1.0f, const float spot_cone_angle_inner = 0.0f,
                           const float spot_cone_angle_outer = 0.78539816339f)
        : type{type}, unit{unit}, color{color}, intensity{intensity}, range{range},
          spot_cone_angle_inner{spot_cone_angle_inner}, spot_cone_angle_outer{spot_cone_angle_outer}
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

struct EnvironmentLight
{
    std::array<float, 3> color{1.0f, 1.0f, 1.0f};
    float intensity{1.0f};
    LightUnit unit{LightUnit::NIT};
    Quaternion rotation{};
    TextureHandle texture{};
};
} // namespace kirana::scene

#endif // KIRANA_SCENE_LIGHT_HPP