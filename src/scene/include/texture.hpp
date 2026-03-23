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

    bool operator==(const TextureSampler &rhs) const
    {
        return mag_filter == rhs.mag_filter && min_filter == rhs.min_filter && mip_map_mode == rhs.mip_map_mode &&
               wrap_mode_u == rhs.wrap_mode_u && wrap_mode_v == rhs.wrap_mode_v && max_anisotropy == rhs.max_anisotropy;
    }
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
    uint32_t tex_coord{0u};

    Texture() = default;

    explicit Texture(const ImageHandle image, const TextureSampler &sampler, const TextureTransform &transform,
                     const uint32_t tex_coord)
        : image{image}, sampler{sampler}, transform{transform}, tex_coord{tex_coord}
    {

    }

    [[nodiscard]] bool isValid() const override
    {
        return IResource::isValid() && image.isValid();
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
}

#endif //KIRANA_SCENE_TEXTURE_HPP