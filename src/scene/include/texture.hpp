// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_TEXTURE_HPP
#define KIRANA_SCENE_TEXTURE_HPP

#include "common.hpp"

#include <resource_manager.hpp>

namespace kirana::scene
{
enum class TextureFilterMode: uint8_t
{
    NEAREST = 0u,
    LINEAR = 1u,
};

enum class TextureWrapMode: uint8_t
{
    REPEAT = 0u,
    MIRRORED_REPEAT = 1u,
    CLAMP_TO_EDGE = 2u,
    CLAMP_TO_BORDER = 3u,
    MIRRORED_CLAMP_TO_EDGE = 4u,
};

struct TextureSampler
{
    TextureFilterMode mag_filter{TextureFilterMode::NEAREST};
    TextureFilterMode min_filter{TextureFilterMode::NEAREST};
    TextureFilterMode mip_map_mode{TextureFilterMode::NEAREST};
    TextureWrapMode wrap_mode_u{TextureWrapMode::REPEAT};
    TextureWrapMode wrap_mode_v{TextureWrapMode::REPEAT};
    uint8_t max_anisotropy{0u};
};

struct TextureTransform
{
    std::array<float, 2> offset{0.0f, 0.0f};
    std::array<float, 2> scale{1.0f, 1.0f};
    float rotation{0.0f};
};

struct Texture final : core::IResource
{
    ImageHandle image{};
    TextureSampler sampler{};
    TextureTransform transform{};
};
}

#endif //KIRANA_SCENE_TEXTURE_HPP