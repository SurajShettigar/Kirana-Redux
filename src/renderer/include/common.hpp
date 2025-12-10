// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_COMMON_HPP
#define KIRANA_RENDERER_COMMON_HPP

#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <version_code.hpp>

namespace kirana::renderer
{
enum class IndexType : uint8_t
{
    NONE = 0u,
    UINT8 = 1u,
    UINT16 = 2u,
    UINT32 = 3u,
};

struct Size2D
{
    uint32_t width = 0;
    uint32_t height = 0;

    [[nodiscard]] bool isValid() const
    {
        return width != 0 && height != 0;
    }
};

struct Size3D
{
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t depth = 0;

    [[nodiscard]] bool isValid() const
    {
        return width != 0 && height != 0 && depth != 0;
    }
};

struct Offset2D
{
    int32_t x = 0;
    int32_t y = 0;
};

struct Offset3D
{
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
};

struct Rect2D
{
    Offset2D offset{};
    Size2D size{};

    [[nodiscard]] bool isValid() const
    {
        return size.isValid();
    }
};

struct Rect3D
{
    Offset3D offset{};
    Size3D size{};

    [[nodiscard]] bool isValid() const
    {
        return size.isValid();
    }
};

struct BufferRegion
{
    uint64_t offset = 0;
    uint64_t size = 0;
};

struct BufferCopyRegion
{
    uint64_t src_offset = 0;
    uint64_t dst_offset = 0;
    uint64_t size = 0;
};

struct TextureCopyRegion
{
    Offset2D src_offset;
    Offset2D dst_offset;
    Size2D size;
};

enum class BufferUsageFlags : uint32_t
{
    UNKNOWN = 0x0u,
    TRANSFER_SRC = 0x00000001u,
    TRANSFER_DST = 0x00000002u,
    UNIFORM_TEXEL_BUFFER = 0x00000004u,
    STORAGE_TEXEL_BUFFER = 0x00000008u,
    UNIFORM_BUFFER = 0x00000010u,
    STORAGE_BUFFER = 0x00000020u,
    INDEX_BUFFER = 0x00000040u,
    VERTEX_BUFFER = 0x00000080u,
    INDIRECT_BUFFER = 0x00000100u,
    SHADER_DEVICE_ADDRESS = 0x00020000u,
    VIDEO_DECODE_SRC = 0x00002000u,
    VIDEO_DECODE_DST = 0x00004000u,
    TRANSFORM_FEEDBACK_BUFFER = 0x00000800u,
    TRANSFORM_FEEDBACK_COUNTER_BUFFER = 0x00001000u,
    CONDITIONAL_RENDERING = 0x00000200u,
    ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY = 0x00080000u,
    ACCELERATION_STRUCTURE_STORAGE = 0x00100000u,
    SHADER_BINDING_TABLE = 0x00000400u,
    VIDEO_ENCODE_DST = 0x00008000u,
    VIDEO_ENCODE_SRC = 0x00010000u,
    SAMPLER_DESCRIPTOR_BUFFER = 0x00200000u,
    RESOURCE_DESCRIPTOR_BUFFER = 0x00400000u,
    PUSH_DESCRIPTORS_DESCRIPTOR_BUFFER = 0x04000000u,
    MICROMAP_BUILD_INPUT_READ_ONLY = 0x00800000u,
    MICROMAP_STORAGE = 0x01000000u,
};

constexpr BufferUsageFlags operator|(const BufferUsageFlags lhs, const BufferUsageFlags rhs)
{
    return static_cast<BufferUsageFlags>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}

constexpr BufferUsageFlags &operator|=(BufferUsageFlags &lhs, const BufferUsageFlags rhs)
{
    return lhs = lhs | rhs;
}

constexpr BufferUsageFlags operator&(const BufferUsageFlags lhs, const BufferUsageFlags rhs)
{
    return static_cast<BufferUsageFlags>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
}

constexpr BufferUsageFlags &operator&=(BufferUsageFlags &lhs, const BufferUsageFlags rhs)
{
    return lhs = lhs & rhs;
}

constexpr BufferUsageFlags operator^(const BufferUsageFlags lhs, const BufferUsageFlags rhs)
{
    return static_cast<BufferUsageFlags>(static_cast<uint32_t>(lhs) ^ static_cast<uint32_t>(rhs));
}

constexpr BufferUsageFlags &operator^=(BufferUsageFlags &lhs, const BufferUsageFlags rhs)
{
    return lhs = lhs ^ rhs;
}

constexpr BufferUsageFlags operator~(const BufferUsageFlags flag)
{
    return static_cast<BufferUsageFlags>(~static_cast<uint32_t>(flag));
}

constexpr bool hasFlag(const BufferUsageFlags flags, const BufferUsageFlags req_flag)
{
    return (flags & req_flag) == req_flag;
}

enum class TextureFormat
{
    UNKNOWN = 0,

    R8_UNORM = 1,
    R8_SNORM = 2,
    R8_USCALED = 3,
    R8_SSCALED = 4,
    R8_UINT = 5,
    R8_SINT = 6,
    R8_SRGB = 7,

    R8G8_UNORM = 8,
    R8G8_SNORM = 9,
    R8G8_USCALED = 10,
    R8G8_SSCALED = 11,
    R8G8_UINT = 12,
    R8G8_SINT = 13,
    R8G8_SRGB = 14,

    R8G8B8_UNORM = 15,
    R8G8B8_SNORM = 16,
    R8G8B8_USCALED = 17,
    R8G8B8_SSCALED = 18,
    R8G8B8_UINT = 19,
    R8G8B8_SINT = 20,
    R8G8B8_SRGB = 21,

    B8G8R8_UNORM = 22,
    B8G8R8_SNORM = 23,
    B8G8R8_USCALED = 24,
    B8G8R8_SSCALED = 25,
    B8G8R8_UINT = 26,
    B8G8R8_SINT = 27,
    B8G8R8_SRGB = 28,

    R8G8B8A8_UNORM = 29,
    R8G8B8A8_SNORM = 30,
    R8G8B8A8_USCALED = 31,
    R8G8B8A8_SSCALED = 32,
    R8G8B8A8_UINT = 33,
    R8G8B8A8_SINT = 34,
    R8G8B8A8_SRGB = 35,

    B8G8R8A8_UNORM = 36,
    B8G8R8A8_SNORM = 37,
    B8G8R8A8_USCALED = 38,
    B8G8R8A8_SSCALED = 39,
    B8G8R8A8_UINT = 40,
    B8G8R8A8_SINT = 41,
    B8G8R8A8_SRGB = 42,

    R16_UNORM = 43,
    R16_SNORM = 44,
    R16_USCALED = 45,
    R16_SSCALED = 46,
    R16_UINT = 47,
    R16_SINT = 48,
    R16_SFLOAT = 49,

    R16G16_UNORM = 50,
    R16G16_SNORM = 51,
    R16G16_USCALED = 52,
    R16G16_SSCALED = 53,
    R16G16_UINT = 54,
    R16G16_SINT = 55,
    R16G16_SFLOAT = 56,

    R16G16B16_UNORM = 57,
    R16G16B16_SNORM = 58,
    R16G16B16_USCALED = 59,
    R16G16B16_SSCALED = 60,
    R16G16B16_UINT = 61,
    R16G16B16_SINT = 62,
    R16G16B16_SFLOAT = 63,

    R16G16B16A16_UNORM = 64,
    R16G16B16A16_SNORM = 65,
    R16G16B16A16_USCALED = 66,
    R16G16B16A16_SSCALED = 67,
    R16G16B16A16_UINT = 68,
    R16G16B16A16_SINT = 69,
    R16G16B16A16_SFLOAT = 70,

    R32_UINT = 71,
    R32_SINT = 72,
    R32_SFLOAT = 73,

    R32G32_UINT = 74,
    R32G32_SINT = 75,
    R32G32_SFLOAT = 76,

    R32G32B32_UINT = 77,
    R32G32B32_SINT = 78,
    R32G32B32_SFLOAT = 79,

    R32G32B32A32_UINT = 80,
    R32G32B32A32_SINT = 81,
    R32G32B32A32_SFLOAT = 82,

    R64_UINT = 83,
    R64_SINT = 84,
    R64_SFLOAT = 85,

    R64G64_UINT = 86,
    R64G64_SINT = 87,
    R64G64_SFLOAT = 88,

    R64G64B64_UINT = 89,
    R64G64B64_SINT = 90,
    R64G64B64_SFLOAT = 91,

    R64G64B64A64_UINT = 92,
    R64G64B64A64_SINT = 93,
    R64G64B64A64_SFLOAT = 94,

    D16_UNORM = 95,
    D32_SFLOAT = 96,
    S8_UINT = 97,
    D16_UNORM_S8_UINT = 98,
    D24_UNORM_S8_UINT = 99,
    D32_SFLOAT_S8_UINT = 100,
};

constexpr bool isDepthTextureFormat(const TextureFormat format)
{
    if (format == TextureFormat::D16_UNORM || format == TextureFormat::D32_SFLOAT ||
        format == TextureFormat::D16_UNORM_S8_UINT || format == TextureFormat::D24_UNORM_S8_UINT ||
        format == TextureFormat::D32_SFLOAT_S8_UINT)
    {
        return true;
    }
    return false;
}

constexpr bool isSRGBTextureFormat(const TextureFormat format)
{
    if (format == TextureFormat::R8_SRGB || format == TextureFormat::R8G8_SRGB ||
        format == TextureFormat::R8G8B8_SRGB || format == TextureFormat::R8G8B8A8_SRGB ||
        format == TextureFormat::B8G8R8_SRGB || format == TextureFormat::B8G8R8A8_SRGB)
    {
        return true;
    }
    return false;
}

enum class TextureLayout
{
    UNKNOWN = 0,
    GENERAL = 1,
    COLOR_ATTACHMENT_OPTIMAL = 2,
    DEPTH_STENCIL_ATTACHMENT_OPTIMAL = 3,
    DEPTH_STENCIL_READ_ONLY_OPTIMAL = 4,
    SHADER_READ_ONLY_OPTIMAL = 5,
    TRANSFER_SRC_OPTIMAL = 6,
    TRANSFER_DST_OPTIMAL = 7,
    PREINITIALIZED = 8,
    DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL = 9,
    DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL = 10,
    DEPTH_ATTACHMENT_OPTIMAL = 11,
    DEPTH_READ_ONLY_OPTIMAL = 12,
    STENCIL_ATTACHMENT_OPTIMAL = 13,
    STENCIL_READ_ONLY_OPTIMAL = 14,
    READ_ONLY_OPTIMAL = 15,
    ATTACHMENT_OPTIMAL = 16,
    RENDERING_LOCAL_READ = 17,
    PRESENT_SRC = 18,
    VIDEO_DECODE_DST = 19,
    VIDEO_DECODE_SRC = 20,
    VIDEO_DECODE_DPB = 21,
    SHARED_PRESENT = 22,
    FRAGMENT_DENSITY_MAP_OPTIMAL = 23,
    FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL = 24,
    SHADING_RATE_OPTIMAL = 25,
    VIDEO_ENCODE_DST = 26,
    VIDEO_ENCODE_SRC = 27,
    VIDEO_ENCODE_DPB = 28,
    ATTACHMENT_FEEDBACK_LOOP_OPTIMAL = 29,
    VIDEO_ENCODE_QUANTIZATION_MAP = 30
};

enum class TextureUsageFlags : uint32_t
{
    UNKNOWN = 0x00000000u,
    TRANSFER_SRC = 0x00000001u,
    TRANSFER_DST = 0x00000002u,
    SAMPLED = 0x00000004u,
    STORAGE = 0x00000008u,
    COLOR_ATTACHMENT = 0x00000010u,
    DEPTH_STENCIL_ATTACHMENT = 0x00000020u,
    TRANSIENT_ATTACHMENT = 0x00000040u,
    INPUT_ATTACHMENT = 0x00000080u,
    HOST_TRANSFER = 0x00400000u,
    VIDEO_DECODE_DST = 0x00000400u,
    VIDEO_DECODE_SRC = 0x00000800u,
    VIDEO_DECODE_DPB = 0x00001000u,
    FRAGMENT_DENSITY_MAP = 0x00000200u,
    FRAGMENT_SHADING_RATE_ATTACHMENT = 0x00000100u,
    VIDEO_ENCODE_DST = 0x00002000u,
    VIDEO_ENCODE_SRC = 0x00004000u,
    VIDEO_ENCODE_DPB = 0x00008000u,
    ATTACHMENT_FEEDBACK_LOOP = 0x00080000u,
    VIDEO_ENCODE_QUANTIZATION_DELTA_MAP = 0x02000000u,
    VIDEO_ENCODE_EMPHASIS_MAP = 0x04000000u,
};

constexpr TextureUsageFlags operator|(const TextureUsageFlags lhs, const TextureUsageFlags rhs)
{
    return static_cast<TextureUsageFlags>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}

constexpr TextureUsageFlags &operator|=(TextureUsageFlags &lhs, const TextureUsageFlags rhs)
{
    return lhs = lhs | rhs;
}

constexpr TextureUsageFlags operator&(const TextureUsageFlags lhs, const TextureUsageFlags rhs)
{
    return static_cast<TextureUsageFlags>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
}

constexpr TextureUsageFlags &operator&=(TextureUsageFlags &lhs, const TextureUsageFlags rhs)
{
    return lhs = lhs & rhs;
}

constexpr TextureUsageFlags operator^(const TextureUsageFlags lhs, const TextureUsageFlags rhs)
{
    return static_cast<TextureUsageFlags>(static_cast<uint32_t>(lhs) ^ static_cast<uint32_t>(rhs));
}

constexpr TextureUsageFlags &operator^=(TextureUsageFlags &lhs, const TextureUsageFlags rhs)
{
    return lhs = lhs ^ rhs;
}

constexpr TextureUsageFlags operator~(const TextureUsageFlags flag)
{
    return static_cast<TextureUsageFlags>(~static_cast<uint32_t>(flag));
}

constexpr bool hasFlag(const TextureUsageFlags flags, const TextureUsageFlags req_flag)
{
    return (flags & req_flag) == req_flag;
}

enum class SampleCountFlags : uint8_t
{
    NONE = 0u,
    S_1 = 1u << 0u,
    S_2 = 1u << 1u,
    S_4 = 1u << 2u,
    S_8 = 1u << 3u,
    S_16 = 1u << 4u,
    S_32 = 1u << 5u,
    S_64 = 1u << 6u,
};

constexpr SampleCountFlags operator|(const SampleCountFlags lhs, const SampleCountFlags rhs)
{
    return static_cast<SampleCountFlags>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

constexpr SampleCountFlags &operator|=(SampleCountFlags &lhs, const SampleCountFlags rhs)
{
    return lhs = lhs | rhs;
}

constexpr SampleCountFlags operator&(const SampleCountFlags lhs, const SampleCountFlags rhs)
{
    return static_cast<SampleCountFlags>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

constexpr SampleCountFlags &operator&=(SampleCountFlags &lhs, const SampleCountFlags rhs)
{
    return lhs = lhs & rhs;
}

constexpr SampleCountFlags operator^(const SampleCountFlags lhs, const SampleCountFlags rhs)
{
    return static_cast<SampleCountFlags>(static_cast<uint8_t>(lhs) ^ static_cast<uint8_t>(rhs));
}

constexpr SampleCountFlags &operator^=(SampleCountFlags &lhs, const SampleCountFlags rhs)
{
    return lhs = lhs ^ rhs;
}

constexpr SampleCountFlags operator~(const SampleCountFlags flag)
{
    return static_cast<SampleCountFlags>(~static_cast<uint8_t>(flag));
}

constexpr bool hasFlag(const SampleCountFlags flags, const SampleCountFlags req_flag)
{
    return (flags & req_flag) == req_flag;
}

enum class SamplerFilterMode : uint8_t
{
    NEAREST = 0u,
    LINEAR = 1u,
};

enum class SamplerWrapMode : uint8_t
{
    REPEAT = 0u,
    MIRRORED_REPEAT = 1u,
    CLAMP_TO_EDGE = 2u,
    CLAMP_TO_BORDER = 3u,
    MIRROR_CLAMP_TO_EDGE = 4u,
};

enum class ShaderStageFlags : uint32_t
{
    NONE = 0x0u,
    VERTEX = 0x00000001u,
    TESSELLATION_CONTROL = 0x00000002u,
    TESSELLATION_EVALUATION = 0x00000004u,
    GEOMETRY = 0x00000008u,
    FRAGMENT = 0x00000010u,
    COMPUTE = 0x00000020u,
    ALL_GRAPHICS = 0x0000001Fu,
    RAYGEN = 0x00000100u,
    ANY_HIT = 0x00000200u,
    CLOSEST_HIT = 0x00000400u,
    MISS = 0x00000800u,
    INTERSECTION = 0x00001000u,
    CALLABLE = 0x00002000u,
    TASK = 0x00000040u,
    MESH = 0x00000080u,
    ALL = 0x7FFFFFFFu,
};

constexpr ShaderStageFlags operator|(const ShaderStageFlags lhs, const ShaderStageFlags rhs)
{
    return static_cast<ShaderStageFlags>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}

constexpr ShaderStageFlags &operator|=(ShaderStageFlags &lhs, const ShaderStageFlags rhs)
{
    return lhs = lhs | rhs;
}

constexpr ShaderStageFlags operator&(const ShaderStageFlags lhs, const ShaderStageFlags rhs)
{
    return static_cast<ShaderStageFlags>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
}

constexpr ShaderStageFlags &operator&=(ShaderStageFlags &lhs, const ShaderStageFlags rhs)
{
    return lhs = lhs & rhs;
}

constexpr ShaderStageFlags operator^(const ShaderStageFlags lhs, const ShaderStageFlags rhs)
{
    return static_cast<ShaderStageFlags>(static_cast<uint32_t>(lhs) ^ static_cast<uint32_t>(rhs));
}

constexpr ShaderStageFlags &operator^=(ShaderStageFlags &lhs, const ShaderStageFlags rhs)
{
    return lhs = lhs ^ rhs;
}

constexpr ShaderStageFlags operator~(const ShaderStageFlags flag)
{
    return static_cast<ShaderStageFlags>(~static_cast<uint32_t>(flag));
}

constexpr bool hasFlag(const ShaderStageFlags flags, const ShaderStageFlags req_flag)
{
    return (flags & req_flag) == req_flag;
}

enum class SubgroupFeatureFlags : uint32_t
{
    NONE = 0u,
    BASIC = 0x00000001u,
    VOTE = 0x00000002u,
    ARITHMETIC = 0x00000004u,
    BALLOT = 0x00000008u,
    SHUFFLE = 0x00000010u,
    SHUFFLE_RELATIVE = 0x00000020u,
    CLUSTERED = 0x00000040u,
    QUAD = 0x00000080u,
    PARTITIONED = 0x00000100u,
    ROTATE = 0x00000200u,
    ROTATE_CLUSTERED = 0x00000400u,
};

constexpr SubgroupFeatureFlags operator|(const SubgroupFeatureFlags lhs, const SubgroupFeatureFlags rhs)
{
    return static_cast<SubgroupFeatureFlags>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}

constexpr SubgroupFeatureFlags &operator|=(SubgroupFeatureFlags &lhs, const SubgroupFeatureFlags rhs)
{
    return lhs = lhs | rhs;
}

constexpr SubgroupFeatureFlags operator&(const SubgroupFeatureFlags lhs, const SubgroupFeatureFlags rhs)
{
    return static_cast<SubgroupFeatureFlags>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
}

constexpr SubgroupFeatureFlags &operator&=(SubgroupFeatureFlags &lhs, const SubgroupFeatureFlags rhs)
{
    return lhs = lhs & rhs;
}

constexpr SubgroupFeatureFlags operator^(const SubgroupFeatureFlags lhs, const SubgroupFeatureFlags rhs)
{
    return static_cast<SubgroupFeatureFlags>(static_cast<uint32_t>(lhs) ^ static_cast<uint32_t>(rhs));
}

constexpr SubgroupFeatureFlags &operator^=(SubgroupFeatureFlags &lhs, const SubgroupFeatureFlags rhs)
{
    return lhs = lhs ^ rhs;
}

constexpr SubgroupFeatureFlags operator~(const SubgroupFeatureFlags flag)
{
    return static_cast<SubgroupFeatureFlags>(~static_cast<uint32_t>(flag));
}

constexpr bool hasFlag(const SubgroupFeatureFlags flags, const SubgroupFeatureFlags req_flag)
{
    return (flags & req_flag) == req_flag;
}

enum class ResolveModeFlags : uint8_t
{
    NONE = 0x00u,
    SAMPLE_ZERO = 0x01u,
    AVERAGE = 0x02u,
    MIN = 0x04u,
    MAX = 0x08u,
};

constexpr ResolveModeFlags operator|(const ResolveModeFlags lhs, const ResolveModeFlags rhs)
{
    return static_cast<ResolveModeFlags>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

constexpr ResolveModeFlags &operator|=(ResolveModeFlags &lhs, const ResolveModeFlags rhs)
{
    return lhs = lhs | rhs;
}

constexpr ResolveModeFlags operator&(const ResolveModeFlags lhs, const ResolveModeFlags rhs)
{
    return static_cast<ResolveModeFlags>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

constexpr ResolveModeFlags &operator&=(ResolveModeFlags &lhs, const ResolveModeFlags rhs)
{
    return lhs = lhs & rhs;
}

constexpr ResolveModeFlags operator^(const ResolveModeFlags lhs, const ResolveModeFlags rhs)
{
    return static_cast<ResolveModeFlags>(static_cast<uint8_t>(lhs) ^ static_cast<uint8_t>(rhs));
}

constexpr ResolveModeFlags &operator^=(ResolveModeFlags &lhs, const ResolveModeFlags rhs)
{
    return lhs = lhs ^ rhs;
}

constexpr ResolveModeFlags operator~(const ResolveModeFlags flag)
{
    return static_cast<ResolveModeFlags>(~static_cast<uint8_t>(flag));
}

constexpr bool hasFlag(const ResolveModeFlags flags, const ResolveModeFlags req_flag)
{
    return (flags & req_flag) == req_flag;
}

enum class ShaderBindingType
{
    UNKNOWN = 0,
    SAMPLER = 1,
    COMBINED_IMAGE_SAMPLER = 2,
    SAMPLED_IMAGE = 3,
    STORAGE_IMAGE = 4,
    UNIFORM_TEXEL_BUFFER = 5,
    STORAGE_TEXEL_BUFFER = 6,
    UNIFORM_BUFFER = 7,
    STORAGE_BUFFER = 8,
    UNIFORM_BUFFER_DYNAMIC = 9,
    STORAGE_BUFFER_DYNAMIC = 10,
    INPUT_ATTACHMENT = 11,
    INLINE_UNIFORM_BLOCK = 12,
    ACCELERATION_STRUCTURE = 13,
    MUTABLE = 14,
};

struct ShaderBinding
{
    uint32_t index = 0;
    ShaderBindingType type = ShaderBindingType::UNKNOWN;
    ShaderStageFlags shader_stages = ShaderStageFlags::NONE;
    uint32_t count = 1;
    bool bindless = false;
};

enum class MemoryAccessFlags : uint64_t
{
    NONE = 0x0u,
    INDIRECT_COMMAND_READ = 0x00000001ull,
    INDEX_READ = 0x00000002ull,
    VERTEX_ATTRIBUTE_READ = 0x00000004ull,
    UNIFORM_READ = 0x00000008ull,
    INPUT_ATTACHMENT_READ = 0x00000010ull,
    SHADER_READ = 0x00000020ull,
    SHADER_WRITE = 0x00000040ull,
    COLOR_ATTACHMENT_READ = 0x00000080ull,
    COLOR_ATTACHMENT_WRITE = 0x00000100ull,
    DEPTH_STENCIL_ATTACHMENT_READ = 0x00000200ull,
    DEPTH_STENCIL_ATTACHMENT_WRITE = 0x00000400ull,
    TRANSFER_READ = 0x00000800ull,
    TRANSFER_WRITE = 0x00001000ull,
    HOST_READ = 0x00002000ull,
    HOST_WRITE = 0x00004000ull,
    MEMORY_READ = 0x00008000ull,
    MEMORY_WRITE = 0x00010000ull,
    COMMAND_PREPROCESS_READ = 0x00020000ull,
    COMMAND_PREPROCESS_WRITE = 0x00040000ull,
    COLOR_ATTACHMENT_READ_NONCOHERENT = 0x00080000ull,
    CONDITIONAL_RENDERING_READ = 0x00100000ull,
    ACCELERATION_STRUCTURE_READ = 0x00200000ull,
    ACCELERATION_STRUCTURE_WRITE = 0x00400000ull,
    FRAGMENT_SHADING_RATE_ATTACHMENT_READ = 0x00800000ull,
    FRAGMENT_DENSITY_MAP_READ = 0x01000000ull,
    TRANSFORM_FEEDBACK_WRITE = 0x02000000ull,
    TRANSFORM_FEEDBACK_COUNTER_READ = 0x04000000ull,
    TRANSFORM_FEEDBACK_COUNTER_WRITE = 0x08000000ull,
    SHADER_SAMPLED_READ = 0x100000000ull,
    SHADER_STORAGE_READ = 0x200000000ull,
    SHADER_STORAGE_WRITE = 0x400000000ull,
    VIDEO_DECODE_READ = 0x800000000ull,
    VIDEO_DECODE_WRITE = 0x1000000000ull,
    VIDEO_ENCODE_READ = 0x2000000000ull,
    VIDEO_ENCODE_WRITE = 0x4000000000ull,
    SHADER_BINDING_TABLE_READ = 0x10000000000ull,
    DESCRIPTOR_BUFFER_READ = 0x20000000000ull,
    MICROMAP_READ = 0x100000000000ull,
    MICROMAP_WRITE = 0x200000000000ull,
};

constexpr MemoryAccessFlags operator|(const MemoryAccessFlags lhs, const MemoryAccessFlags rhs)
{
    return static_cast<MemoryAccessFlags>(static_cast<uint64_t>(lhs) | static_cast<uint64_t>(rhs));
}

constexpr MemoryAccessFlags &operator|=(MemoryAccessFlags &lhs, const MemoryAccessFlags rhs)
{
    return lhs = lhs | rhs;
}

constexpr MemoryAccessFlags operator&(const MemoryAccessFlags lhs, const MemoryAccessFlags rhs)
{
    return static_cast<MemoryAccessFlags>(static_cast<uint64_t>(lhs) & static_cast<uint64_t>(rhs));
}

constexpr MemoryAccessFlags &operator&=(MemoryAccessFlags &lhs, const MemoryAccessFlags rhs)
{
    return lhs = lhs & rhs;
}

constexpr MemoryAccessFlags operator^(const MemoryAccessFlags lhs, const MemoryAccessFlags rhs)
{
    return static_cast<MemoryAccessFlags>(static_cast<uint64_t>(lhs) ^ static_cast<uint64_t>(rhs));
}

constexpr MemoryAccessFlags &operator^=(MemoryAccessFlags &lhs, const MemoryAccessFlags rhs)
{
    return lhs = lhs ^ rhs;
}

constexpr MemoryAccessFlags operator~(const MemoryAccessFlags lhs)
{
    return static_cast<MemoryAccessFlags>(~static_cast<uint64_t>(lhs));
}

constexpr bool hasFlag(const MemoryAccessFlags flags, const MemoryAccessFlags req_flag)
{
    return (flags & req_flag) == req_flag;
}

enum class PipelineStageFlags : uint64_t
{
    NONE = 0ull,
    TOP_OF_PIPE = 0x00000001ull,
    DRAW_INDIRECT = 0x00000002ull,
    VERTEX_INPUT = 0x00000004ull,
    VERTEX_SHADER = 0x00000008ull,
    TESSELLATION_CONTROL_SHADER = 0x00000010ull,
    TESSELLATION_EVALUATION_SHADER = 0x00000020ull,
    GEOMETRY_SHADER = 0x00000040ull,
    FRAGMENT_SHADER = 0x00000080ull,
    EARLY_FRAGMENT_TESTS = 0x00000100ull,
    LATE_FRAGMENT_TESTS = 0x00000200ull,
    COLOR_ATTACHMENT_OUTPUT = 0x00000400ull,
    COMPUTE_SHADER = 0x00000800ull,
    ALL_TRANSFER = 0x00001000ull,
    TRANSFER = 0x00001000ull,
    BOTTOM_OF_PIPE = 0x00002000ull,
    HOST = 0x00004000ull,
    ALL_GRAPHICS = 0x00008000ull,
    ALL_COMMANDS = 0x00010000ull,
    COPY = 0x100000000ull,
    RESOLVE = 0x200000000ull,
    BLIT = 0x400000000ull,
    CLEAR = 0x800000000ull,
    INDEX_INPUT = 0x1000000000ull,
    VERTEX_ATTRIBUTE_INPUT = 0x2000000000ull,
    PRE_RASTERIZATION_SHADERS = 0x4000000000ull,
    VIDEO_DECODE = 0x04000000ull,
    VIDEO_ENCODE = 0x08000000ull,
    TRANSFORM_FEEDBACK = 0x01000000ull,
    CONDITIONAL_RENDERING = 0x00040000ull,
    COMMAND_PREPROCESS = 0x00020000ull,
    FRAGMENT_SHADING_RATE_ATTACHMENT = 0x00400000ull,
    ACCELERATION_STRUCTURE_BUILD = 0x02000000ull,
    RAY_TRACING_SHADER = 0x00200000ull,
    FRAGMENT_DENSITY_PROCESS = 0x00800000ull,
    TASK_SHADER = 0x00080000ull,
    MESH_SHADER = 0x00100000ull,
    ACCELERATION_STRUCTURE_COPY = 0x10000000ull,
    MICROMAP_BUILD = 0x40000000ull,
};

constexpr PipelineStageFlags operator|(const PipelineStageFlags lhs, const PipelineStageFlags rhs)
{
    return static_cast<PipelineStageFlags>(static_cast<uint64_t>(lhs) | static_cast<uint64_t>(rhs));
}

constexpr PipelineStageFlags &operator|=(PipelineStageFlags &lhs, const PipelineStageFlags rhs)
{
    return lhs = lhs | rhs;
}

constexpr PipelineStageFlags operator&(const PipelineStageFlags lhs, const PipelineStageFlags rhs)
{
    return static_cast<PipelineStageFlags>(static_cast<uint64_t>(lhs) & static_cast<uint64_t>(rhs));
}

constexpr PipelineStageFlags &operator&=(PipelineStageFlags &lhs, const PipelineStageFlags rhs)
{
    return lhs = lhs & rhs;
}

constexpr PipelineStageFlags operator^(const PipelineStageFlags lhs, const PipelineStageFlags rhs)
{
    return static_cast<PipelineStageFlags>(static_cast<uint64_t>(lhs) ^ static_cast<uint64_t>(rhs));
}

constexpr PipelineStageFlags &operator^=(PipelineStageFlags &lhs, const PipelineStageFlags rhs)
{
    return lhs = lhs ^ rhs;
}

constexpr PipelineStageFlags operator~(const PipelineStageFlags lhs)
{
    return static_cast<PipelineStageFlags>(~static_cast<uint64_t>(lhs));
}

constexpr bool hasFlag(const PipelineStageFlags flags, const PipelineStageFlags req_flag)
{
    return (flags & req_flag) == req_flag;
}

enum class PipelineType : uint8_t
{
    GRAPHICS = 0,
    COMPUTE = 1,
    RAY_TRACING = 2,
};

enum class QueueFamilyFlags
{
    NONE = 0,
    GRAPHICS = 1 << 0,
    PRESENTATION = 1 << 1,
    COMPUTE = 1 << 2,
    TRANSFER = 1 << 3,
};

constexpr QueueFamilyFlags operator|(const QueueFamilyFlags lhs, const QueueFamilyFlags rhs)
{
    return static_cast<QueueFamilyFlags>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}

constexpr QueueFamilyFlags &operator|=(QueueFamilyFlags &lhs, const QueueFamilyFlags rhs)
{
    return lhs = lhs | rhs;
}

constexpr QueueFamilyFlags operator&(const QueueFamilyFlags lhs, const QueueFamilyFlags rhs)
{
    return static_cast<QueueFamilyFlags>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
}

constexpr QueueFamilyFlags &operator&=(QueueFamilyFlags &lhs, const QueueFamilyFlags rhs)
{
    return lhs = lhs & rhs;
}

constexpr QueueFamilyFlags operator^(const QueueFamilyFlags lhs, const QueueFamilyFlags rhs)
{
    return static_cast<QueueFamilyFlags>(static_cast<uint32_t>(lhs) ^ static_cast<uint32_t>(rhs));
}

constexpr QueueFamilyFlags &operator^=(QueueFamilyFlags &lhs, const QueueFamilyFlags rhs)
{
    return lhs = lhs ^ rhs;
}

constexpr QueueFamilyFlags operator~(const QueueFamilyFlags flag)
{
    return static_cast<QueueFamilyFlags>(~static_cast<uint32_t>(flag));
}

constexpr bool hasFlag(const QueueFamilyFlags flags, const QueueFamilyFlags req_flag)
{
    return (flags & req_flag) == req_flag;
}

struct QueueFamily
{
    uint32_t index = std::numeric_limits<uint32_t>::max();
    uint32_t num_queues = 0;
    QueueFamilyFlags support_flags = QueueFamilyFlags::NONE;

    [[nodiscard]] bool isValid() const
    {
        return index < std::numeric_limits<uint32_t>::max() && num_queues > 0 &&
               support_flags != QueueFamilyFlags::NONE;
    }

    [[nodiscard]] bool supportsRendering() const
    {
        return hasFlag(support_flags, QueueFamilyFlags::GRAPHICS);
    }

    [[nodiscard]] bool supportsPresentation() const
    {
        return hasFlag(support_flags, QueueFamilyFlags::PRESENTATION);
    }

    [[nodiscard]] bool supportsCompute() const
    {
        return hasFlag(support_flags, QueueFamilyFlags::COMPUTE);
    }

    [[nodiscard]] bool supportsTransfer() const
    {
        return hasFlag(support_flags, QueueFamilyFlags::TRANSFER);
    }

    [[nodiscard]] bool supportsSurfaceRendering() const
    {
        return supportsRendering() && supportsPresentation();
    }

    [[nodiscard]] bool supportsOnlyCompute() const
    {
        return supportsCompute() && !supportsRendering();
    }

    [[nodiscard]] bool supportsOnlyTransfer() const
    {
        return supportsTransfer() && !(supportsCompute() || supportsRendering());
    }
};

enum class SwapchainBufferMode
{
    UNKNOWN = 0,
    SINGLE = 1,
    DOUBLE = 2,
    TRIPLE = 3,
};

enum class PresentMode
{
    UNKNOWN = 0,
    IMMEDIATE = 1,
    MAILBOX = 2,
    FIFO = 3,
    FIFO_RELAXED = 4,
};

struct SwapchainData
{
    Size2D size{};
    TextureFormat format{TextureFormat::UNKNOWN};
    SwapchainBufferMode buffer_mode{SwapchainBufferMode::DOUBLE};
    PresentMode present_mode{PresentMode::IMMEDIATE};
};

enum class GPUType
{
    UNKNOWN = 0,
    INTEGRATED = 1,
    DISCRETE = 2,
};

struct GPUFeatures
{
    // Base Features
    bool robust_buffer_access = false;
    bool full_draw_index_uint_32 = false;
    bool image_cube_array = false;
    bool independent_blend = false;
    bool geometry_shader = false;
    bool tessellation_shader = false;
    bool sample_rate_shading = false;
    bool dual_src_blend = false;
    bool logic_op = false;
    bool multi_draw_indirect = false;
    bool draw_indirect_first_instance = false;
    bool depth_clamp = false;
    bool depth_bias_clamp = false;
    bool fill_mode_non_solid = false;
    bool depth_bounds = false;
    bool wide_lines = false;
    bool large_points = false;
    bool alpha_to_one = false;
    bool multi_viewport = false;
    bool sampler_anisotropy = false;
    bool texture_compression_ETC2 = false;
    bool texture_compression_ASTCLDR = false;
    bool texture_compression_BC = false;
    bool occlusion_query_precise = false;
    bool pipeline_statistics_query = false;
    bool vertex_pipeline_stores_and_atomics = false;
    bool fragment_stores_and_atomics = false;
    bool shader_tessellation_and_geometry_point_size = false;
    bool shader_image_gather_extended = false;
    bool shader_storage_image_extended_formats = false;
    bool shader_storage_image_multisample = false;
    bool shader_storage_image_read_without_format = false;
    bool shader_storage_image_write_without_format = false;
    bool shader_uniform_buffer_array_dynamic_indexing = false;
    bool shader_sampled_image_array_dynamic_indexing = false;
    bool shader_storage_buffer_array_dynamic_indexing = false;
    bool shader_storage_image_array_dynamic_indexing = false;
    bool shader_clip_distance = false;
    bool shader_cull_distance = false;
    bool shader_float_64 = false;
    bool shader_int_64 = false;
    bool shader_int_16 = false;
    bool shader_resource_residency = false;
    bool shader_resource_min_lod = false;
    bool sparse_binding = false;
    bool sparse_residency_buffer = false;
    bool sparse_residency_image_2D = false;
    bool sparse_residency_image_3D = false;
    bool sparse_residency_2_samples = false;
    bool sparse_residency_4_samples = false;
    bool sparse_residency_8_samples = false;
    bool sparse_residency_16_samples = false;
    bool sparse_residency_aliased = false;
    bool variable_multisample_rate = false;
    bool inherited_queries = false;
    // Vulkan 1.1 Features
    bool storage_buffer_16_bit_access = false;
    bool uniform_and_storage_buffer_16_bit_access = false;
    bool storage_push_constant_16 = false;
    bool storage_input_output_16 = false;
    bool multiview = false;
    bool multiview_geometry_shader = false;
    bool multiview_tessellation_shader = false;
    bool variable_pointers_storage_buffer = false;
    bool variable_pointers = false;
    bool protected_memory = false;
    bool sampler_YCBCR_conversion = false;
    bool shader_draw_parameters = false;
    // Vulkan 1.2 Features
    bool sampler_mirror_clamp_to_edge = false;
    bool draw_indirect_count = false;
    bool storage_buffer_8_bit_access = false;
    bool uniform_and_storage_buffer_8_bit_access = false;
    bool storage_push_constant_8 = false;
    bool shader_buffer_int_64_atomics = false;
    bool shader_shared_int_64_atomics = false;
    bool shader_float_16 = false;
    bool shader_int_8 = false;
    bool descriptor_indexing = false;
    bool shader_input_attachment_array_dynamic_indexing = false;
    bool shader_uniform_texel_buffer_array_dynamic_indexing = false;
    bool shader_storage_texel_buffer_array_dynamic_indexing = false;
    bool shader_uniform_buffer_array_non_uniform_indexing = false;
    bool shader_sampled_image_array_non_uniform_indexing = false;
    bool shader_storage_buffer_array_non_uniform_indexing = false;
    bool shader_storage_image_array_non_uniform_indexing = false;
    bool shader_input_attachment_array_non_uniform_indexing = false;
    bool shader_uniform_texel_buffer_array_non_uniform_indexing = false;
    bool shader_storage_texel_buffer_array_non_uniform_indexing = false;
    bool descriptor_binding_uniform_buffer_update_after_bind = false;
    bool descriptor_binding_sampled_image_update_after_bind = false;
    bool descriptor_binding_storage_image_update_after_bind = false;
    bool descriptor_binding_storage_buffer_update_after_bind = false;
    bool descriptor_binding_uniform_texel_buffer_update_after_bind = false;
    bool descriptor_binding_storage_texel_buffer_update_after_bind = false;
    bool descriptor_binding_update_unused_while_pending = false;
    bool descriptor_binding_partially_bound = false;
    bool descriptor_binding_variable_descriptor_count = false;
    bool runtime_descriptor_array = false;
    bool sampler_filter_minmax = false;
    bool scalar_block_layout = false;
    bool imageless_framebuffer = false;
    bool uniform_buffer_standard_layout = false;
    bool shader_subgroup_extended_types = false;
    bool separate_depth_stencil_layouts = false;
    bool host_query_reset = false;
    bool timeline_semaphore = false;
    bool buffer_device_address = false;
    bool buffer_device_address_capture_replay = false;
    bool buffer_device_address_multi_device = false;
    bool vulkan_memory_model = false;
    bool vulkan_memory_model_device_scope = false;
    bool vulkan_memory_model_availability_visibility_chains = false;
    bool shader_output_viewport_index = false;
    bool shader_output_layer = false;
    bool subgroup_broadcast_dynamic_id = false;
    // Vulkan 1.3 Features
    bool robust_image_access = false;
    bool inline_uniform_block = false;
    bool descriptor_binding_inline_uniform_block_update_after_bind = false;
    bool pipeline_creation_cache_control = false;
    bool private_data = false;
    bool shader_demote_to_helper_invocation = false;
    bool shader_terminate_invocation = false;
    bool subgroup_size_control = false;
    bool compute_full_subgroups = false;
    bool synchronization_2 = false;
    bool texture_compression_ASTC_HDR = false;
    bool shader_zero_initialize_workgroup_memory = false;
    bool dynamic_rendering = false;
    bool shader_integer_dot_product = false;
    bool maintenance_4 = false;
    // Vulkan 1.4 Features
    bool global_priority_query = false;
    bool shader_subgroup_rotate = false;
    bool shader_subgroup_rotate_clustered = false;
    bool shader_float_controls_2 = false;
    bool shader_expect_assume = false;
    bool rectangular_lines = false;
    bool bresenham_lines = false;
    bool smooth_lines = false;
    bool stippled_rectangular_lines = false;
    bool stippled_bresenham_lines = false;
    bool stippled_smooth_lines = false;
    bool vertex_attribute_instance_rate_divisor = false;
    bool vertex_attribute_instance_rate_zero_divisor = false;
    bool index_type_uint_8 = false;
    bool dynamic_rendering_local_read = false;
    bool maintenance_5 = false;
    bool maintenance_6 = false;
    bool pipeline_protected_access = false;
    bool pipeline_robustness = false;
    bool host_image_copy = false;
    bool push_descriptor = false;
    // Acceleration Structure features
    bool acceleration_structure = false;
    bool acceleration_structure_capture_replay = false;
    bool acceleration_structure_indirect_build = false;
    bool acceleration_structure_host_commands = false;
    bool descriptor_binding_acceleration_structure_update_after_bind = false;
    // Raytracing features
    bool ray_tracing_pipeline = false;
    bool ray_tracing_pipeline_shader_group_handle_capture_replay = false;
    bool ray_tracing_pipeline_shader_group_handle_capture_replay_mixed = false;
    bool ray_tracing_pipeline_trace_rays_indirect = false;
    bool ray_traversal_primitive_culling = false;
};

struct GPULimits
{
    uint32_t max_image_dimension_1D = 0;
    uint32_t max_image_dimension_2D = 0;
    uint32_t max_image_dimension_3D = 0;
    uint32_t max_image_dimension_cube = 0;
    uint32_t max_image_array_layers = 0;
    uint32_t max_texel_buffer_elements = 0;
    uint32_t max_uniform_buffer_range = 0;
    uint32_t max_storage_buffer_range = 0;
    uint32_t max_push_constants_size = 0;
    uint32_t max_memory_allocation_count = 0;
    uint32_t max_sampler_allocation_count = 0;
    uint64_t buffer_image_granularity = 0;
    uint64_t sparse_address_space_size = 0;
    uint32_t max_bound_descriptor_sets = 0;
    uint32_t max_per_stage_descriptor_samplers = 0;
    uint32_t max_per_stage_descriptor_uniform_buffers = 0;
    uint32_t max_per_stage_descriptor_storage_buffers = 0;
    uint32_t max_per_stage_descriptor_sampled_images = 0;
    uint32_t max_per_stage_descriptor_storage_images = 0;
    uint32_t max_per_stage_descriptor_input_attachments = 0;
    uint32_t max_per_stage_resources = 0;
    uint32_t max_descriptor_set_samplers = 0;
    uint32_t max_descriptor_set_uniform_buffers = 0;
    uint32_t max_descriptor_set_uniform_buffers_dynamic = 0;
    uint32_t max_descriptor_set_storage_buffers = 0;
    uint32_t max_descriptor_set_storage_buffers_dynamic = 0;
    uint32_t max_descriptor_set_sampled_images = 0;
    uint32_t max_descriptor_set_storage_images = 0;
    uint32_t max_descriptor_set_input_attachments = 0;
    uint32_t max_vertex_input_attributes = 0;
    uint32_t max_vertex_input_bindings = 0;
    uint32_t max_vertex_input_attribute_offset = 0;
    uint32_t max_vertex_input_binding_stride = 0;
    uint32_t max_vertex_output_components = 0;
    uint32_t max_tessellation_generation_level = 0;
    uint32_t max_tessellation_patch_size = 0;
    uint32_t max_tessellation_control_per_vertex_input_components = 0;
    uint32_t max_tessellation_control_per_vertex_output_components = 0;
    uint32_t max_tessellation_control_per_patch_output_components = 0;
    uint32_t max_tessellation_control_total_output_components = 0;
    uint32_t max_tessellation_evaluation_input_components = 0;
    uint32_t max_tessellation_evaluation_output_components = 0;
    uint32_t max_geometry_shader_invocations = 0;
    uint32_t max_geometry_input_components = 0;
    uint32_t max_geometry_output_components = 0;
    uint32_t max_geometry_output_vertices = 0;
    uint32_t max_geometry_total_output_components = 0;
    uint32_t max_fragment_input_components = 0;
    uint32_t max_fragment_output_attachments = 0;
    uint32_t max_fragment_dual_src_attachments = 0;
    uint32_t max_fragment_combined_output_resources = 0;
    uint32_t max_compute_shared_memory_size = 0;
    std::array<uint32_t, 3> max_compute_work_group_count{0, 0, 0};
    uint32_t max_compute_work_group_invocations = 0;
    std::array<uint32_t, 3> max_compute_work_group_size{0, 0, 0};
    uint32_t sub_pixel_precision_bits = 0;
    uint32_t sub_texel_precision_bits = 0;
    uint32_t mipmap_precision_bits = 0;
    uint32_t max_draw_indexed_index_value = 0;
    uint32_t max_draw_indirect_count = 0;
    float max_sampler_lod_bias = 0.0f;
    float max_sampler_anisotropy = 0.0f;
    uint32_t max_viewports = 0;
    std::array<uint32_t, 2> max_viewport_dimensions{0, 0};
    std::array<float, 2> viewport_bounds_range{0.0f, 0.0f};
    uint32_t viewport_sub_pixel_bits = 0;
    size_t min_memory_map_alignment = 0;
    uint64_t min_texel_buffer_offset_alignment = 0;
    uint64_t min_uniform_buffer_offset_alignment = 0;
    uint64_t min_storage_buffer_offset_alignment = 0;
    int32_t min_texel_offset = 0;
    uint32_t max_texel_offset = 0;
    int32_t min_texel_gather_offset = 0;
    uint32_t max_texel_gather_offset = 0;
    float min_interpolation_offset = 0.0f;
    float max_interpolation_offset = 0.0f;
    uint32_t sub_pixel_interpolation_offset_bits = 0;
    uint32_t max_framebuffer_width = 0;
    uint32_t max_framebuffer_height = 0;
    uint32_t max_framebuffer_layers = 0;
    SampleCountFlags framebuffer_color_sample_counts = SampleCountFlags::NONE;
    SampleCountFlags framebuffer_depth_sample_counts = SampleCountFlags::NONE;
    SampleCountFlags framebuffer_stencil_sample_counts = SampleCountFlags::NONE;
    SampleCountFlags framebuffer_no_attachments_sample_counts = SampleCountFlags::NONE;
    uint32_t max_color_attachments = 0;
    SampleCountFlags sampled_image_color_sample_counts = SampleCountFlags::NONE;
    SampleCountFlags sampled_image_integer_sample_counts = SampleCountFlags::NONE;
    SampleCountFlags sampled_image_depth_sample_counts = SampleCountFlags::NONE;
    SampleCountFlags sampled_image_stencil_sample_counts = SampleCountFlags::NONE;
    SampleCountFlags storage_image_sample_counts = SampleCountFlags::NONE;
    uint32_t max_sample_mask_words = 0;
    bool timestamp_compute_and_graphics = false;
    float timestamp_period = 0.0f;
    uint32_t max_clip_distances = 0;
    uint32_t max_cull_distances = 0;
    uint32_t max_combined_clip_and_cull_distances = 0;
    uint32_t discrete_queue_priorities = 0;
    std::array<float, 2> point_size_range{0.0f, 0.0f};
    std::array<float, 2> line_width_range{0.0f, 0.0f};
    float point_size_granularity = 0.0f;
    float line_width_granularity = 0.0f;
    bool strict_lines = false;
    bool standard_sample_locations = false;
    uint64_t optimal_buffer_copy_offset_alignment = 0;
    uint64_t optimal_buffer_copy_row_pitch_alignment = 0;
    uint64_t non_coherent_atom_size = 0;
    // Vulkan 1.1 Limits
    uint32_t subgroup_size = 0;
    ShaderStageFlags subgroup_supported_stages = ShaderStageFlags::NONE;
    SubgroupFeatureFlags subgroup_supported_operations = SubgroupFeatureFlags::NONE;
    bool subgroup_quad_operations_in_all_stages = false;
    uint32_t point_clipping_behavior = 0;
    uint32_t max_multiview_view_count = 0;
    uint32_t max_multiview_instance_index = 0;
    bool protected_no_fault = false;
    uint32_t max_per_set_descriptors = 0;
    uint64_t max_memory_allocation_size = 0;
    // Vulkan 1.2 Limits
    uint32_t denorm_behavior_independence = 0;
    uint32_t rounding_mode_independence = 0;
    bool shader_signed_zero_inf_nan_preserve_float_16 = false;
    bool shader_signed_zero_inf_nan_preserve_float_32 = false;
    bool shader_signed_zero_inf_nan_preserve_float_64 = false;
    bool shader_denorm_preserve_float_16 = false;
    bool shader_denorm_preserve_float_32 = false;
    bool shader_denorm_preserve_float_64 = false;
    bool shader_denorm_flush_to_zero_float_16 = false;
    bool shader_denorm_flush_to_zero_float_32 = false;
    bool shader_denorm_flush_to_zero_float_64 = false;
    bool shader_rounding_mode_RTE_float_16 = false;
    bool shader_rounding_mode_RTE_float_32 = false;
    bool shader_rounding_mode_RTE_float_64 = false;
    bool shader_rounding_mode_RTZ_float_16 = false;
    bool shader_rounding_mode_RTZ_float_32 = false;
    bool shader_rounding_mode_RTZ_float_64 = false;
    uint32_t max_update_after_bind_descriptors_in_all_pools = 0;
    bool shader_uniform_buffer_array_non_uniform_indexing_native = false;
    bool shader_sampled_image_array_non_uniform_indexing_native = false;
    bool shader_storage_buffer_array_non_uniform_indexing_native = false;
    bool shader_storage_image_array_non_uniform_indexing_native = false;
    bool shader_input_attachment_array_non_uniform_indexing_native = false;
    bool robust_buffer_access_update_after_bind = false;
    bool quad_divergent_implicit_lod = false;
    uint32_t max_per_stage_descriptor_update_after_bind_samplers = 0;
    uint32_t max_per_stage_descriptor_update_after_bind_uniform_buffers = 0;
    uint32_t max_per_stage_descriptor_update_after_bind_storage_buffers = 0;
    uint32_t max_per_stage_descriptor_update_after_bind_sampled_images = 0;
    uint32_t max_per_stage_descriptor_update_after_bind_storage_images = 0;
    uint32_t max_per_stage_descriptor_update_after_bind_input_attachments = 0;
    uint32_t max_per_stage_update_after_bind_resources = 0;
    uint32_t max_descriptor_set_update_after_bind_samplers = 0;
    uint32_t max_descriptor_set_update_after_bind_uniform_buffers = 0;
    uint32_t max_descriptor_set_update_after_bind_uniform_buffers_dynamic = 0;
    uint32_t max_descriptor_set_update_after_bind_storage_buffers = 0;
    uint32_t max_descriptor_set_update_after_bind_storage_buffers_dynamic = 0;
    uint32_t max_descriptor_set_update_after_bind_sampled_images = 0;
    uint32_t max_descriptor_set_update_after_bind_storage_images = 0;
    uint32_t max_descriptor_set_update_after_bind_input_attachments = 0;
    ResolveModeFlags supported_depth_resolve_modes = ResolveModeFlags::NONE;
    ResolveModeFlags supported_stencil_resolve_modes = ResolveModeFlags::NONE;
    bool independent_resolve_none = false;
    bool independent_resolve = false;
    bool filter_minmax_single_component_formats = false;
    bool filter_minmax_image_component_mapping = false;
    uint64_t max_timeline_semaphore_value_difference = 0;
    SampleCountFlags framebuffer_integer_color_sample_counts = SampleCountFlags::NONE;
    // Vulkan 1.3 Limits
    uint32_t min_subgroup_size = 0;
    uint32_t max_subgroup_size = 0;
    uint32_t max_compute_workgroup_subgroups = 0;
    ShaderStageFlags required_subgroup_size_stages = ShaderStageFlags::NONE;
    uint32_t max_inlineUniformBlockSize = 0;
    uint32_t max_per_stage_descriptor_inline_uniform_blocks = 0;
    uint32_t max_per_stage_descriptor_update_after_bind_inline_uniform_blocks = 0;
    uint32_t max_descriptor_set_inline_uniform_blocks = 0;
    uint32_t max_descriptor_set_update_after_bind_inline_uniform_blocks = 0;
    uint32_t max_inline_uniform_total_size = 0;
    bool integer_dot_product_8_bit_unsigned_accelerated = false;
    bool integer_dot_product_8_bit_signed_accelerated = false;
    bool integer_dot_product_8_bit_mixed_signedness_accelerated = false;
    bool integer_dot_product_4x8_bit_packed_unsigned_accelerated = false;
    bool integer_dot_product_4x8_bit_packed_signed_accelerated = false;
    bool integer_dot_product_4x8_bit_packed_mixed_signedness_accelerated = false;
    bool integer_dot_product_16_bit_unsigned_accelerated = false;
    bool integer_dot_product_16_bit_signed_accelerated = false;
    bool integer_dot_product_16_bit_mixed_signedness_accelerated = false;
    bool integer_dot_product_32_bit_unsigned_accelerated = false;
    bool integer_dot_product_32_bit_signed_accelerated = false;
    bool integer_dot_product_32_bit_mixed_signedness_accelerated = false;
    bool integer_dot_product_64_bit_unsigned_accelerated = false;
    bool integer_dot_product_64_bit_signed_accelerated = false;
    bool integer_dot_product_64_bit_mixed_signedness_accelerated = false;
    bool integer_dot_product_accumulating_saturating_8_bit_unsigned_accelerated = false;
    bool integer_dot_product_accumulating_saturating_8_bit_signed_accelerated = false;
    bool integer_dot_product_accumulating_saturating_8_bit_mixed_signedness_accelerated = false;
    bool integer_dot_product_accumulating_saturating_4x8_bit_packed_unsigned_accelerated = false;
    bool integer_dot_product_accumulating_saturating_4x8_bit_packed_signed_accelerated = false;
    bool integer_dot_product_accumulating_saturating_4x8_bit_packed_mixed_signedness_accelerated = false;
    bool integer_dot_product_accumulating_saturating_16_bit_unsigned_accelerated = false;
    bool integer_dot_product_accumulating_saturating_16_bit_signed_accelerated = false;
    bool integer_dot_product_accumulating_saturating_16_bit_mixed_signedness_accelerated = false;
    bool integer_dot_product_accumulating_saturating_32_bit_unsigned_accelerated = false;
    bool integer_dot_product_accumulating_saturating_32_bit_signed_accelerated = false;
    bool integer_dot_product_accumulating_saturating_32_bit_mixed_signedness_accelerated = false;
    bool integer_dot_product_accumulating_saturating_64_bit_unsigned_accelerated = false;
    bool integer_dot_product_accumulating_saturating_64_bit_signed_accelerated = false;
    bool integer_dot_product_accumulating_saturating_64_bit_mixed_signedness_accelerated = false;
    uint64_t storage_texel_buffer_offset_alignment_bytes = 0;
    bool storage_texel_buffer_offset_single_texel_alignment = false;
    uint64_t uniform_texel_buffer_offset_alignment_bytes = 0;
    bool uniform_texel_buffer_offset_single_texel_alignment = false;
    uint64_t max_buffer_size = 0;
    // Vulkan 1.4 Limits
    uint32_t line_sub_pixel_precision_bits = 0;
    uint32_t max_vertex_attrib_divisor = 0;
    bool supports_non_zero_first_instance = false;
    uint32_t max_push_descriptors = 0;
    bool dynamic_rendering_local_read_depth_stencil_attachments = false;
    bool dynamic_rendering_local_read_multisampled_attachments = false;
    bool early_fragment_multisample_coverage_after_sample_counting = false;
    bool early_fragment_sample_mask_test_before_sample_counting = false;
    bool depth_stencil_swizzle_one_support = false;
    bool polygon_mode_point_size = false;
    bool non_strict_single_pixel_wide_lines_use_parallelogram = false;
    bool non_strict_wide_lines_use_parallelogram = false;
    bool block_texel_view_compatible_multiple_layers = false;
    uint32_t max_combined_image_sampler_descriptor_count = 0;
    bool fragment_shading_rate_clamp_combiner_inputs = false;
    uint32_t default_robustness_storage_buffers = 0;
    uint32_t default_robustness_uniform_buffers = 0;
    uint32_t default_robustness_vertex_inputs = 0;
    uint32_t default_robustness_images = 0;
    std::vector<TextureLayout> copy_src_layouts = {};
    std::vector<TextureLayout> copy_dst_layouts = {};
    std::array<uint8_t, 16u> optimal_tiling_layout_UUID = {};
    bool identical_memory_type_requirements = false;
    // Acceleration Structure limits
    uint64_t max_geometry_count = 0;
    uint64_t max_instance_count = 0;
    uint64_t max_primitive_count = 0;
    uint32_t max_per_stage_descriptor_acceleration_structures = 0;
    uint32_t max_per_stage_descriptor_update_after_bind_acceleration_structures = 0;
    uint32_t max_descriptor_set_acceleration_structures = 0;
    uint32_t max_descriptor_set_update_after_bind_acceleration_structures = 0;
    uint32_t min_acceleration_structure_scratch_offset_alignment = 0;
    // Raytracing limits
    uint32_t shader_group_handle_size = 0;
    uint32_t max_ray_recursion_depth = 0;
    uint32_t max_shader_group_stride = 0;
    uint32_t shader_group_base_alignment = 0;
    uint32_t shader_group_handle_capture_replay_size = 0;
    uint32_t max_ray_dispatch_invocation_count = 0;
    uint32_t shader_group_handle_alignment = 0;
    uint32_t max_ray_hit_attribute_size = 0;
};

struct SurfaceData
{
    /// HWND for Windows API, Window for X-LIB API.
    void *window_handle{nullptr};
    /// HINSTANCE for Windows API, Display for X-LIB API.
    void *instance_handle{nullptr};

    [[nodiscard]] bool isValid() const
    {
        return window_handle != nullptr && instance_handle != nullptr;
    }
};

struct GPUSelectionPreference
{
    GPUType type = GPUType::UNKNOWN;
    GPUFeatures features = {};
    GPULimits limits = {};
    QueueFamilyFlags queue_types = QueueFamilyFlags::NONE;
    bool dedicated_compute_queue = false;
    bool dedicated_transfer_queue = false;
};

struct DeviceInitializationData
{
    bool debug_mode = {false};
    std::string app_name = {};
    core::VersionCode version = {};
    SurfaceData surface = {};
    GPUSelectionPreference gpu_preference = {};
};
} // namespace kirana::renderer
#endif // KIRANA_RENDERER_COMMON_HPP