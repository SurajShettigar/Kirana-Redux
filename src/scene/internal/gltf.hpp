// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_SCENE_GLTF_HPP
#define KIRANA_SCENE_GLTF_HPP

#include <glaze/glaze.hpp>

#include <span>

namespace kirana::scene
{
static const std::string LOG_CHANNEL_GLTF = "GLTF";

#pragma region GLTF_STRUCTS
using GLTF_INT_8 = int8_t;
using GLTF_UINT_8 = uint8_t;
using GLTF_INT_16 = int16_t;
using GLTF_UINT_16 = uint16_t;
using GLTF_UINT_32 = uint32_t;
using GLTF_FLOAT = float;

using GLTF_VEC_2 = std::array<GLTF_FLOAT, 2>;
using GLTF_VEC_3 = std::array<GLTF_FLOAT, 3>;
using GLTF_VEC_4 = std::array<GLTF_FLOAT, 4>;
using GLTF_MAT_2 = std::array<GLTF_FLOAT, 4>;
using GLTF_MAT_3 = std::array<GLTF_FLOAT, 9>;
using GLTF_MAT_4 = std::array<GLTF_FLOAT, 16>;

using GLTF_VEC_2i8 = std::array<GLTF_INT_8, 2>;
using GLTF_VEC_3i8 = std::array<GLTF_INT_8, 3>;
using GLTF_VEC_4i8 = std::array<GLTF_INT_8, 4>;
using GLTF_MAT_2i8 = std::array<GLTF_INT_8, 4>;
using GLTF_MAT_3i8 = std::array<GLTF_INT_8, 9>;
using GLTF_MAT_4i8 = std::array<GLTF_INT_8, 16>;

using GLTF_VEC_2u8 = std::array<GLTF_UINT_8, 2>;
using GLTF_VEC_3u8 = std::array<GLTF_UINT_8, 3>;
using GLTF_VEC_4u8 = std::array<GLTF_UINT_8, 4>;
using GLTF_MAT_2u8 = std::array<GLTF_UINT_8, 4>;
using GLTF_MAT_3u8 = std::array<GLTF_UINT_8, 9>;
using GLTF_MAT_4u8 = std::array<GLTF_UINT_8, 16>;

using GLTF_VEC_2i16 = std::array<GLTF_INT_16, 2>;
using GLTF_VEC_3i16 = std::array<GLTF_INT_16, 3>;
using GLTF_VEC_4i16 = std::array<GLTF_INT_16, 4>;
using GLTF_MAT_2i16 = std::array<GLTF_INT_16, 4>;
using GLTF_MAT_3i16 = std::array<GLTF_INT_16, 9>;
using GLTF_MAT_4i16 = std::array<GLTF_INT_16, 16>;

using GLTF_VEC_2u16 = std::array<GLTF_UINT_16, 2>;
using GLTF_VEC_3u16 = std::array<GLTF_UINT_16, 3>;
using GLTF_VEC_4u16 = std::array<GLTF_UINT_16, 4>;
using GLTF_MAT_2u16 = std::array<GLTF_UINT_16, 4>;
using GLTF_MAT_3u16 = std::array<GLTF_UINT_16, 9>;
using GLTF_MAT_4u16 = std::array<GLTF_UINT_16, 16>;

using GLTF_VEC_2u = std::array<GLTF_UINT_32, 2>;
using GLTF_VEC_3u = std::array<GLTF_UINT_32, 3>;
using GLTF_VEC_4u = std::array<GLTF_UINT_32, 4>;
using GLTF_MAT_2u = std::array<GLTF_UINT_32, 4>;
using GLTF_MAT_3u = std::array<GLTF_UINT_32, 9>;
using GLTF_MAT_4u = std::array<GLTF_UINT_32, 16>;

static const std::string GLTF_EXT_NAME_DRACO_MESH_COMPRESSION = "KHR_draco_mesh_compression";
static const std::string GLTF_EXT_NAME_LIGHTS_PUNCTUAL = "KHR_lights_punctual";
static const std::string GLTF_EXT_NAME_MAT_ANISOTROPY = "KHR_materials_anisotropy";
static const std::string GLTF_EXT_NAME_MAT_CLEARCOAT = "KHR_materials_clearcoat";
static const std::string GLTF_EXT_NAME_MAT_DIFFUSE_TRANSMISSION = "KHR_materials_diffuse_transmission";
static const std::string GLTF_EXT_NAME_MAT_DISPERSION = "KHR_materials_dispersion";
static const std::string GLTF_EXT_NAME_MAT_EMISSIVE_STRENGTH = "KHR_materials_emissive_strength";
static const std::string GLTF_EXT_NAME_MAT_IOR = "KHR_materials_ior";
static const std::string GLTF_EXT_NAME_MAT_IRIDESCENCE = "KHR_materials_iridescence";
static const std::string GLTF_EXT_NAME_MAT_SHEEN = "KHR_materials_sheen";
static const std::string GLTF_EXT_NAME_MAT_SPECULAR = "KHR_materials_specular";
static const std::string GLTF_EXT_NAME_MAT_TRANSMISSION = "KHR_materials_transmission";
static const std::string GLTF_EXT_NAME_MAT_UNLIT = "KHR_materials_unlit";
static const std::string GLTF_EXT_NAME_MAT_VOLUME = "KHR_materials_volume";
static const std::string GLTF_EXT_NAME_TEXTURE_TRANSFORM = "KHR_texture_transform";


enum class GLTFComponentType : uint32_t
{
    UNKNOWN = 0u,
    INT_8 = 5120u,
    UINT_8 = 5121u,
    INT_16 = 5122u,
    UINT_16 = 5123u,
    UINT_32 = 5125u,
    FLOAT = 5126u
};

enum class GLTFAccessorType: uint32_t
{
    UNKNOWN = 0u,
    SCALAR = 1u,
    VEC_2 = 2u,
    VEC_3 = 3u,
    VEC_4 = 4u,
    MAT_2 = 5u,
    MAT_3 = 6u,
    MAT_4 = 7u
};

enum class GLTFPrimitiveAttribute: uint32_t
{
    UNKNOWN = 0u,
    POSITION = 1u,
    NORMAL = 2u,
    TANGENT = 3u,
    TEX_COORD = 4u,
    COLOR = 5u,
    JOINTS = 6u,
    WEIGHTS = 7u,
};

enum class GLTFPrimitiveMode : uint32_t
{
    POINTS = 0u,
    LINES = 1u,
    LINE_LOOP = 2u,
    LINE_STRIP = 3u,
    TRIANGLES = 4u,
    TRIANGLE_STRIP = 5u,
    TRIANGLE_FAN = 6u
};

enum class GLTFBufferTarget : uint32_t
{
    UNKNOWN = 0u,
    ARRAY_BUFFER = 34962u,
    ELEMENT_ARRAY_BUFFER = 34963u
};

enum class GLTFImageMimeType : uint32_t
{
    UNKNOWN = 0u,
    JPEG = 1u,
    PNG = 2u,
};

enum class GLTFFilter : uint32_t
{
    UNKNOWN = 0u,
    NEAREST = 9728u,
    LINEAR = 9729u,
    NEAREST_MIPMAP_NEAREST = 9984u,
    LINEAR_MIPMAP_NEAREST = 9985u,
    NEAREST_MIPMAP_LINEAR = 9986u,
    LINEAR_MIPMAP_LINEAR = 9987u
};

enum class GLTFWrapMode : uint32_t
{
    UNKNOWN = 0u,
    CLAMP_TO_EDGE = 33071,
    MIRRORED_REPEAT = 33648,
    REPEAT = 10497
};

enum class GLTFCameraType: uint32_t
{
    PERSPECTIVE = 0u,
    ORTHOGRAPHIC = 1u
};

enum class GLTFAlphaMode: uint32_t
{
    OPAQUE = 0u,
    MASK = 1u,
    BLEND = 2u,
};

enum class GLTFPunctualLightType: uint32_t
{
    DIRECTIONAL = 0u,
    POINT = 1u,
    SPOT = 2u,
};

enum class GLTFAnimationPath: uint32_t
{
    UNKNOWN = 0u,
    TRANSLATION = 1u,
    ROTATION = 2u,
    SCALE = 3u,
    WEIGHTS = 4u
};

enum class GLTFInterpolation: uint32_t
{
    UNKNOWN = 0u,
    LINEAR = 1u,
    STEP = 2u,
    CUBIC_SPLINE = 3u,
};

struct GLTFAsset
{
    std::string version{}; // required
    std::string min_version{};
    std::string copyright{};
    std::string generator{};

    [[nodiscard]] bool isValid() const
    {
        return !version.empty();
    }
};

struct GLTFBuffer
{
    uint64_t byte_length{}; // required

    std::optional<std::string> uri{};
    std::optional<std::string> name{};

    [[nodiscard]] bool isValid() const
    {
        return byte_length > 0;
    }
};

struct GLTFBufferView
{
    /// Index into the buffer array.
    uint32_t buffer{}; // required
    uint64_t byte_length{}; // required

    uint64_t byte_offset{0};
    std::optional<uint8_t> byte_stride{};
    std::optional<GLTFBufferTarget> target{};
    std::optional<std::string> name{};

    [[nodiscard]] bool isValid() const
    {
        return byte_length > 0;
    }
};


struct GLTFAccessorSparseIndices
{
    GLTFComponentType component_type{}; // required - must be an unsigned int type
    uint32_t buffer_view{}; // required
    uint64_t byte_offset{0};

    [[nodiscard]] bool isValid() const
    {
        return (component_type == GLTFComponentType::UINT_8 || component_type == GLTFComponentType::UINT_16 ||
                component_type == GLTFComponentType::UINT_32);
    }
};

struct GLTFAccessorSparseValues
{
    uint32_t buffer_view{}; // required
    uint64_t byte_offset{0};
};

struct GLTFAccessorSparse
{
    uint64_t count{}; // required
    GLTFAccessorSparseIndices indices{}; // required
    GLTFAccessorSparseValues values{}; // required

    [[nodiscard]] bool isValid() const
    {
        return count > 0 && indices.isValid();
    }
};

struct GLTFAccessor
{
    GLTFComponentType component_type{}; // required
    GLTFAccessorType type{}; // required
    uint64_t count{}; // required

    std::optional<uint32_t> buffer_view{};
    std::optional<GLTFAccessorSparse> sparse{};
    uint64_t byte_offset{0};
    bool normalized{false};
    // Max and min values can also be integer values depending on component_type. That is why we use double precision
    // floats to be able to represent all 32-bit integer and float values accurately.
    std::vector<double> max{};
    std::vector<double> min{};
    std::optional<std::string> name{};

    [[nodiscard]] bool isValid() const
    {
        return component_type != GLTFComponentType::UNKNOWN && type != GLTFAccessorType::UNKNOWN && count > 0;
    }

    [[nodiscard]] bool isSparse() const
    {
        return !buffer_view && sparse.has_value();
    }

    [[nodiscard]] size_t getPerElementSize() const
    {
        const auto componentSize = [](const GLTFComponentType type) -> size_t {
            switch (type)
            {
            case GLTFComponentType::INT_8:
            case GLTFComponentType::UINT_8:
                return 1;
            case GLTFComponentType::INT_16:
            case GLTFComponentType::UINT_16:
                return 2;
            case GLTFComponentType::UINT_32:
            case GLTFComponentType::FLOAT:
                return 4;
            case GLTFComponentType::UNKNOWN:
            default:
                return 0;
            }
        };
        const auto numComponents = [](const GLTFAccessorType type) -> size_t {
            switch (type)
            {
            case GLTFAccessorType::SCALAR:
                return 1;
            case GLTFAccessorType::VEC_2:
                return 2;
            case GLTFAccessorType::VEC_3:
                return 3;
            case GLTFAccessorType::VEC_4:
            case GLTFAccessorType::MAT_2:
                return 4;
            case GLTFAccessorType::MAT_3:
                return 9;
            case GLTFAccessorType::MAT_4:
                return 16;
            case GLTFAccessorType::UNKNOWN:
            default:
                return 0;
            }
        };
        return componentSize(component_type) * numComponents(type);
    }
};


struct GLTFImage
{
    std::optional<std::string> uri{};
    std::optional<GLTFImageMimeType> mime_type{};
    std::optional<uint32_t> buffer_view{};
    std::optional<std::string> name{};

    [[nodiscard]] bool isValid() const
    {
        return (uri.has_value() && !buffer_view.has_value())
               || (!uri.has_value() && buffer_view.has_value() && mime_type.has_value());
    }
};

struct GLTFSampler
{
    std::optional<GLTFFilter> mag_filter{};
    std::optional<GLTFFilter> min_filter{};
    GLTFWrapMode wrap_s{GLTFWrapMode::REPEAT};
    GLTFWrapMode wrap_t{GLTFWrapMode::REPEAT};
    std::optional<std::string> name{};
};

struct GLTFTexture
{
    /// Index into the sampler array.
    std::optional<uint32_t> sampler{};
    /// Index into the image array.
    std::optional<uint32_t> source{};
    std::optional<std::string> name{};
};

struct GLTFTextureTransform
{
    std::array<float, 2> offset{0.0f, 0.0f};
    std::array<float, 2> scale{1.0f, 1.0f};
    float rotation{0.0f};
    std::optional<uint8_t> tex_coord{};
};

struct GLTFTextureInfo
{
    /// Index into the texture array.
    uint32_t index{}; // required
    /// Index of Texture (UV) coordinates. (TEX_COORD0, TEX_COORD1, etc.)
    uint8_t tex_coord{0};
    std::optional<GLTFTextureTransform> transform{};

    void setExtensions(const std::optional<glz::json_t> &extensions);
    [[nodiscard]] std::optional<glz::json_t> getExtensions() const;
};

struct GLTFTextureInfoNormal
{
    uint32_t index{}; // required
    uint8_t tex_coord{0};
    float scale{1.0f};
    std::optional<GLTFTextureTransform> transform{};

    void setExtensions(const std::optional<glz::json_t> &extensions);
    [[nodiscard]] std::optional<glz::json_t> getExtensions() const;
};

struct GLTFTextureInfoOcclusion
{
    uint32_t index{}; // required
    uint8_t tex_coord{0};
    float strength{1.0f};
    std::optional<GLTFTextureTransform> transform{};

    void setExtensions(const std::optional<glz::json_t> &extensions);
    [[nodiscard]] std::optional<glz::json_t> getExtensions() const;
};

struct GLTFMaterialPBRMetallicRoughness
{
    std::array<float, 4> base_color_factor{1.0f, 1.0f, 1.0f, 1.0f};
    std::optional<GLTFTextureInfo> base_color_texture{};
    float metallic_factor{1.0f};
    float roughness_factor{1.0f};
    std::optional<GLTFTextureInfo> metallic_roughness_texture{};
};

struct GLTFMaterialAnisotropy
{
    float strength{0.0f};
    float rotation{0.0f};
    std::optional<GLTFTextureInfo> texture{};
};

struct GLTFMaterialClearcoat
{
    float factor{0.0f};
    std::optional<GLTFTextureInfo> texture{};
    float roughness_factor{0.0f};
    std::optional<GLTFTextureInfo> roughness_texture{};
    std::optional<GLTFTextureInfoNormal> normal_texture{};
};

struct GLTFMaterialDiffuseTransmission
{
    float factor{0.0f};
    std::optional<GLTFTextureInfo> texture{};
    GLTF_VEC_3 color_factor{1.0f, 1.0f, 1.0f};
    std::optional<GLTFTextureInfo> color_texture{};
};

struct GLTFMaterialDispersion
{
    float dispersion{0.0f};
};

struct GLTFMaterialEmissiveStrength
{
    float strength{1.0f};
};


struct GLTFMaterialIOR
{
    float ior{1.5f};
};

struct GLTFMaterialIridescence
{
    float factor{0.0f};
    std::optional<GLTFTextureInfo> texture{};
    float ior{1.3f};
    float thickness_min{100.0f};
    float thickness_max{400.0f};
    std::optional<GLTFTextureInfo> thickness_texture{};
};

struct GLTFMaterialSheen
{
    GLTF_VEC_3 color_factor{0.0f, 0.0f, 0.0f};
    std::optional<GLTFTextureInfo> color_texture{};
    float roughness_factor{0.0f};
    std::optional<GLTFTextureInfo> roughness_texture{};
};

struct GLTFMaterialSpecular
{
    float factor{0.0f};
    std::optional<GLTFTextureInfo> texture{};
    GLTF_VEC_3 color_factor{1.0f, 1.0f, 1.0f};
    std::optional<GLTFTextureInfo> color_texture{};
};

struct GLTFMaterialTransmission
{
    float factor{0.0f};
    std::optional<GLTFTextureInfo> texture{};
};

struct GLTFMaterialUnlit
{
};

struct GLTFMaterialVolume
{
    float thickness_factor{0.0f};
    std::optional<GLTFTextureInfo> thickness_texture{};
    float attenuation_distance{std::numeric_limits<float>::infinity()};
    GLTF_VEC_3 attenuation_color{1.0f, 1.0f, 1.0f};
};

struct GLTFMaterial
{
    std::optional<std::string> name{};
    std::optional<GLTFMaterialPBRMetallicRoughness> pbr_metallic_roughness{};
    std::optional<GLTFTextureInfoNormal> normal_texture{};
    std::optional<GLTFTextureInfoOcclusion> occlusion_texture{};
    std::optional<GLTFTextureInfo> emissive_texture{};
    std::array<float, 3> emissive_factor{0.0f, 0.0f, 0.0f};
    GLTFAlphaMode alpha_mode{GLTFAlphaMode::OPAQUE};
    float alpha_cutoff{0.5f};
    bool double_sided{false};
    bool unlit{false};
    std::optional<GLTFMaterialAnisotropy> anisotropy{};
    std::optional<GLTFMaterialClearcoat> clearcoat{};
    std::optional<GLTFMaterialDiffuseTransmission> diffuse_transmission{};
    std::optional<GLTFMaterialDispersion> dispersion{};
    std::optional<GLTFMaterialEmissiveStrength> emissive_strength{};
    std::optional<GLTFMaterialIOR> ior{};
    std::optional<GLTFMaterialIridescence> iridescence{};
    std::optional<GLTFMaterialSheen> sheen{};
    std::optional<GLTFMaterialSpecular> specular{};
    std::optional<GLTFMaterialTransmission> transmission{};
    std::optional<GLTFMaterialVolume> volume{};

    void setExtensions(const std::optional<glz::json_t> &extensions);
    [[nodiscard]] std::optional<glz::json_t> getExtensions() const;
};

struct GLTFMeshPrimitive
{
    /// Map of the mesh attribute name and its corresponding accessor index.
    std::map<std::string, uint32_t> attributes{}; // required
    /// Index of the accessor which contain vertex indices.
    std::optional<uint32_t> indices{};
    /// Index into the material array.
    std::optional<uint32_t> material{};
    GLTFPrimitiveMode mode{GLTFPrimitiveMode::TRIANGLES};
    /// Array of morph targets. Each morph target has a mapping of the attribute name and its corresponding accessor index.
    std::map<std::string, uint32_t> targets{};

    [[nodiscard]] bool isValid() const
    {
        return !attributes.empty();
    }

    [[nodiscard]] std::vector<uint32_t> getAttributeAccessor(const GLTFPrimitiveAttribute &attrib) const
    {
        if (attrib == GLTFPrimitiveAttribute::POSITION && attributes.contains("POSITION"))
        {
            return {attributes.at("POSITION")};
        }
        if (attrib == GLTFPrimitiveAttribute::NORMAL && attributes.contains("NORMAL"))
        {
            return {attributes.at("NORMAL")};
        }
        if (attrib == GLTFPrimitiveAttribute::TANGENT && attributes.contains("TANGENT"))
        {
            return {attributes.at("TANGENT")};
        }
        if (attrib == GLTFPrimitiveAttribute::TEX_COORD)
        {
            std::vector<uint32_t> result{};
            for (const auto &[key, value] : attributes)
            {
                if (key.starts_with("TEXCOORD_"))
                {
                    result.push_back(value);
                }
            }
            return result;
        }
        if (attrib == GLTFPrimitiveAttribute::COLOR)
        {
            std::vector<uint32_t> result{};
            for (const auto &[key, value] : attributes)
            {
                if (key.starts_with("COLOR_"))
                {
                    result.push_back(value);
                }
            }
            return result;
        }
        if (attrib == GLTFPrimitiveAttribute::JOINTS)
        {
            std::vector<uint32_t> result{};
            for (const auto &[key, value] : attributes)
            {
                if (key.starts_with("JOINTS_"))
                {
                    result.push_back(value);
                }
            }
            return result;
        }
        if (attrib == GLTFPrimitiveAttribute::WEIGHTS)
        {
            std::vector<uint32_t> result{};
            for (const auto &[key, value] : attributes)
            {
                if (key.starts_with("WEIGHTS_"))
                {
                    result.push_back(value);
                }
            }
            return result;
        }
        return {};
    }
};

struct GLTFMesh
{
    std::vector<GLTFMeshPrimitive> primitives{}; // required
    /// Morph-Target weights.
    std::vector<float> weights{};
    std::optional<std::string> name{};

    [[nodiscard]] bool isValid() const
    {
        return !primitives.empty();
    }
};

struct GLTFCameraOrthographic
{
    float magnification_x{}; // required
    float magnification_y{}; // required
    float z_far{}; // required
    float z_near{}; // required

    [[nodiscard]] bool isValid() const
    {
        return magnification_x > 0.0f && magnification_y > 0.0f && z_far > 0.0f && z_near >= 0.0f && z_far > z_near;
    }
};

struct GLTFCameraPerspective
{
    float fov_y{}; // required
    float z_near{}; // required
    std::optional<float> aspect_ratio{};
    std::optional<float> z_far{};

    [[nodiscard]] bool isValid() const
    {
        return fov_y > 0.0f && z_near > 0.0f && (z_far.has_value() ? *z_far > 0.0f && *z_far > z_near : true) && (
                   aspect_ratio.has_value() ? *aspect_ratio > 0.0f : true);
    }
};

struct GLTFCamera
{
    GLTFCameraType type{}; // required

    std::optional<GLTFCameraOrthographic> orthographic{};
    std::optional<GLTFCameraPerspective> perspective{};
    std::optional<std::string> name{};

    [[nodiscard]] bool isValid() const
    {
        return (type == GLTFCameraType::ORTHOGRAPHIC && orthographic.has_value() && !perspective.has_value()) ||
               (type == GLTFCameraType::PERSPECTIVE && !orthographic.has_value() && perspective.has_value());
    }
};

struct GLTFSpotLight
{
    GLTF_FLOAT cone_angle_inner{0.0f};
    GLTF_FLOAT cone_angle_outer{0.78539816339f};
};

struct GLTFPunctualLight
{
    GLTFPunctualLightType type{}; // required
    GLTF_VEC_3 color{1.0f, 1.0f, 1.0f};
    float intensity{1.0f};
    std::optional<float> range{};
    std::optional<GLTFSpotLight> spot{};
    std::optional<std::string> name{};
};

struct GLTFNode
{
    std::optional<uint32_t> camera{};
    std::vector<uint32_t> children{};
    std::optional<uint32_t> skin{};
    std::optional<uint32_t> mesh{};
    std::optional<uint32_t> light{};

    /// Column-Major local transformation matrix.
    std::optional<GLTF_MAT_4> matrix{};
    /// Unit Quaternion rotation. [x, y, z, w]
    std::optional<GLTF_VEC_4> rotation{};
    std::optional<GLTF_VEC_3> scale{};
    std::optional<GLTF_VEC_3> translation{};
    /// Per mesh instance morph-target weights.
    std::vector<float> weights{};
    std::optional<std::string> name{};

    void setExtensions(const std::optional<glz::json_t> &extensions);
    [[nodiscard]] std::optional<glz::json_t> getExtensions() const;

    [[nodiscard]] bool isValid() const
    {
        return (camera.has_value() && !skin.has_value() && !mesh.has_value())
               || (!camera.has_value() && mesh.has_value())
               || (!camera.has_value() && skin.has_value() && mesh.has_value())
               || (!camera.has_value() && !weights.empty() && mesh.has_value());
    }
};

struct GLTFSkin
{
    /// Node indices which correspond to skin joints.
    std::vector<uint32_t> joints{}; // required
    /// Index into the accessor array containing inverse bind matrices corresponding to each joint.
    std::optional<uint32_t> inverse_bind_matrices{};
    /// Index of the node which is considered the skeleton root.
    std::optional<uint32_t> skeleton{};
    std::optional<std::string> name{};

    [[nodiscard]] bool isValid() const
    {
        return !joints.empty();
    }
};

struct GLTFScene
{
    std::vector<uint32_t> nodes{};
    std::optional<std::string> name{};
};

struct GLTFAnimationChannelTarget
{
    GLTFAnimationPath path{}; // required
    std::optional<uint32_t> node{};

    [[nodiscard]] bool isValid() const
    {
        return path != GLTFAnimationPath::UNKNOWN;
    }
};

struct GLTFAnimationChannel
{
    uint32_t sampler{}; // required
    GLTFAnimationChannelTarget target{}; // required
};

struct GLTFAnimationSampler
{
    /// Accessor index containing timestamps.
    uint32_t input{}; // required
    /// Accessor index containing values.
    uint32_t output{}; // required

    GLTFInterpolation interpolation{GLTFInterpolation::LINEAR};
};

struct GLTFAnimation
{
    std::vector<GLTFAnimationChannel> channels{}; // required
    std::vector<GLTFAnimationSampler> samplers{}; // required

    std::optional<std::string> name{};

    [[nodiscard]] bool isValid() const
    {
        return !channels.empty() && !samplers.empty();
    }
};

struct GLTFDocument
{
    GLTFAsset asset{}; // required
    std::vector<std::string> extensions_used{};
    std::vector<std::string> extensions_required{};
    std::vector<GLTFAccessor> accessors{};
    std::vector<GLTFAnimation> animations{};
    std::vector<GLTFBuffer> buffers{};
    std::vector<GLTFBufferView> buffer_views{};
    std::vector<GLTFCamera> cameras{};
    std::vector<GLTFImage> images{};
    std::vector<GLTFMaterial> materials{};
    std::vector<GLTFMesh> meshes{};
    std::vector<GLTFNode> nodes{};
    std::vector<GLTFSampler> samplers{};
    std::optional<uint32_t> scene{};
    std::vector<GLTFScene> scenes{};
    std::vector<GLTFSkin> skins{};
    std::vector<GLTFTexture> textures{};
    std::vector<GLTFPunctualLight> lights{};

    void setExtensions(const std::optional<glz::json_t> &extensions);
    [[nodiscard]] std::optional<glz::json_t> getExtensions() const;

    [[nodiscard]] bool isValid() const
    {
        return asset.isValid();
    }
};

struct GLTFBinaryHeader
{
    uint32_t magic_number{0};
    uint32_t version{0};
    uint32_t length{0};

    GLTFBinaryHeader() = default;
    ~GLTFBinaryHeader() = default;

    explicit GLTFBinaryHeader(const std::span<uint8_t> file_buffer)
    {
        std::memcpy(this, file_buffer.data(), sizeof(GLTFBinaryHeader));
    }

    [[nodiscard]] bool isValid() const
    {
        // The first 4-bytes of GLB binary contain a magic number which translates to `gLTF` in ASCII.
        return magic_number == 0x46546C67u;
    }
};

enum class GLTFBinaryChunkType: uint32_t
{
    BIN = 0x004E4942u,
    JSON = 0x4E4F534Au,
};

struct GLTFBinaryChunk
{
    uint32_t length{0};
    GLTFBinaryChunkType type{GLTFBinaryChunkType::JSON};
    std::span<uint8_t> buffer_span{};
};

class GLTFBinaryDocument
{
public:
    GLTFBinaryDocument() = default;
    ~GLTFBinaryDocument() = default;

    /// Interprets the buffer as GLTF Binary and returns the formatted data based on the GLTF spec.
    /// Refer: https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#binary-header
    explicit GLTFBinaryDocument(std::span<uint8_t> file_buffer);

    [[nodiscard]] bool isValid() const
    {
        return m_header.isValid();
    }

    [[nodiscard]] std::span<const uint8_t> getJSONBuffer() const
    {
        for (const auto &chunk : m_chunks)
        {
            if (chunk.type == GLTFBinaryChunkType::JSON)
            {
                return chunk.buffer_span;
            }
        }
        return {};
    }

    [[nodiscard]] std::span<const uint8_t> getBinaryBuffer() const
    {
        for (const auto &chunk : m_chunks)
        {
            if (chunk.type == GLTFBinaryChunkType::BIN)
            {
                return chunk.buffer_span;
            }
        }
        return {};
    }

private:
    GLTFBinaryHeader m_header{};
    std::vector<GLTFBinaryChunk> m_chunks{};
};

class GLTFLoader
{
public:
    GLTFLoader() = default;
    ~GLTFLoader() = default;

    /**
     * Loads a GLTF / GLB file.
     * @param path Path to the GLTF / GLB file.
     * @param load_buffers If true, the external binary buffers (.bin) files are loaded into the memory.
     */
    explicit GLTFLoader(std::string path, bool load_buffers = false);

    [[nodiscard]] bool isValid() const
    {
        return m_document.isValid();
    }

    [[nodiscard]] bool isBinary() const
    {
        return m_binary_doc.isValid();
    }

    [[nodiscard]] const GLTFDocument &getDocument() const
    {
        return m_document;
    }

    [[nodiscard]] std::span<const uint8_t> getBuffer(uint32_t buffer_index = 0);

    [[nodiscard]] const std::variant<std::string, std::vector<uint8_t>> &getImage(const uint32_t image_index) const
    {
        return m_images.at(image_index);
    }

    bool loadBufferViewData(const GLTFBufferView &view, uint8_t *out_data);
    bool loadAccessorData(const GLTFAccessor &accessor, uint8_t *out_data);

private:
    std::string m_path{};
    std::string m_base_path{};

    std::vector<uint8_t> m_buffer{};
    std::unordered_map<uint32_t, std::span<const uint8_t>> m_buffer_views{};

    std::unordered_map<uint32_t, std::variant<std::string, std::vector<uint8_t>>> m_images{};

    GLTFDocument m_document{};
    GLTFBinaryDocument m_binary_doc{};

    bool loadBuffer(uint32_t buffer_index);
    bool loadImage(uint32_t image_index);
};
#pragma endregion
}
#pragma region GLAZE_JSON_SCHEMA
template <>
struct glz::meta<kirana::scene::GLTFAccessorType>
{
    using enum kirana::scene::GLTFAccessorType;
    static constexpr auto value = enumerate(
        "SCALAR", SCALAR,
        "VEC2", VEC_2,
        "VEC3", VEC_3,
        "VEC4", VEC_4,
        "MAT2", MAT_2,
        "MAT3", MAT_3,
        "MAT4", MAT_4
        );
};

template <>
struct glz::meta<kirana::scene::GLTFImageMimeType>
{
    using enum kirana::scene::GLTFImageMimeType;
    static constexpr auto value = glz::enumerate(
        "image/jpeg", JPEG,
        "image/png", PNG
        );
};

template <>
struct glz::meta<kirana::scene::GLTFCameraType>
{
    using enum kirana::scene::GLTFCameraType;
    static constexpr auto value = glz::enumerate(
        "perspective", PERSPECTIVE,
        "orthographic", ORTHOGRAPHIC
        );
};

template <>
struct glz::meta<kirana::scene::GLTFPunctualLightType>
{
    using enum kirana::scene::GLTFPunctualLightType;
    static constexpr auto value = glz::enumerate(
        "directional", DIRECTIONAL,
        "point", POINT,
        "spot", SPOT
        );
};

template <>
struct glz::meta<kirana::scene::GLTFAlphaMode>
{
    using enum kirana::scene::GLTFAlphaMode;
    static constexpr auto value = glz::enumerate(
        "OPAQUE", OPAQUE,
        "MASK", MASK,
        "BLEND", BLEND
        );
};


template <>
struct glz::meta<kirana::scene::GLTFAnimationPath>
{
    using enum kirana::scene::GLTFAnimationPath;
    static constexpr auto value = glz::enumerate(
        "translation", TRANSLATION,
        "rotation", ROTATION,
        "scale", SCALE,
        "weights", WEIGHTS
        );
};

template <>
struct glz::meta<kirana::scene::GLTFInterpolation>
{
    using enum kirana::scene::GLTFInterpolation;
    static constexpr auto value = glz::enumerate(
        "LINEAR", LINEAR,
        "STEP", STEP,
        "CUBICSPLINE", CUBIC_SPLINE
        );
};

template <>
struct glz::meta<kirana::scene::GLTFAsset>
{
    using T = kirana::scene::GLTFAsset;
    static constexpr auto value = object(
        "version", &T::version,
        "minVersion", &T::min_version,
        "copyright", &T::copyright,
        "generator", &T::generator
        );
};

template <>
struct glz::meta<kirana::scene::GLTFBuffer>
{
    using T = kirana::scene::GLTFBuffer;
    static constexpr auto value = object(
        "uri", &T::uri,
        "byteLength", &T::byte_length,
        "name", &T::name
        );
};

template <>
struct glz::meta<kirana::scene::GLTFBufferView>
{
    using T = kirana::scene::GLTFBufferView;
    static constexpr auto value = object(
        "buffer", &T::buffer,
        "byteOffset", &T::byte_offset,
        "byteLength", &T::byte_length,
        "byteStride", &T::byte_stride,
        "target", &T::target,
        "name", &T::name
        );
};

template <>
struct glz::meta<kirana::scene::GLTFAccessorSparseIndices>
{
    using T = kirana::scene::GLTFAccessorSparseIndices;
    static constexpr auto value = object(
        "bufferView", &T::buffer_view,
        "byteOffset", &T::byte_offset,
        "componentType", &T::component_type
        );
};

template <>
struct glz::meta<kirana::scene::GLTFAccessorSparseValues>
{
    using T = kirana::scene::GLTFAccessorSparseValues;
    static constexpr auto value = object(
        "bufferView", &T::buffer_view,
        "byteOffset", &T::byte_offset
        );
};

template <>
struct glz::meta<kirana::scene::GLTFAccessorSparse>
{
    using T = kirana::scene::GLTFAccessorSparse;
    static constexpr auto value = object(
        "count", &T::count,
        "indices", &T::indices,
        "values", &T::values
        );
};

template <>
struct glz::meta<kirana::scene::GLTFAccessor>
{
    using T = kirana::scene::GLTFAccessor;
    static constexpr auto value = object(
        "bufferView", &T::buffer_view,
        "byteOffset", &T::byte_offset,
        "componentType", &T::component_type,
        "normalized", &T::normalized,
        "count", &T::count,
        "type", &T::type,
        "max", &T::max,
        "min", &T::min,
        "sparse", &T::sparse,
        "name", &T::name
        );
};

template <>
struct glz::meta<kirana::scene::GLTFImage>
{
    using T = kirana::scene::GLTFImage;
    static constexpr auto value = object(
        "uri", &T::uri,
        "mimeType", &T::mime_type,
        "bufferView", &T::buffer_view,
        "name", &T::name
        );
};

template <>
struct glz::meta<kirana::scene::GLTFSampler>
{
    using T = kirana::scene::GLTFSampler;
    static constexpr auto value = object(
        "magFilter", &T::mag_filter,
        "minFilter", &T::min_filter,
        "wrapS", &T::wrap_s,
        "wrapT", &T::wrap_t,
        "name", &T::name
        );
};

template <>
struct glz::meta<kirana::scene::GLTFTexture>
{
    using T = kirana::scene::GLTFTexture;
    static constexpr auto value = object(
        "sampler", &T::sampler,
        "source", &T::source,
        "name", &T::name
        );
};

template <>
struct glz::meta<kirana::scene::GLTFTextureTransform>
{
    using T = kirana::scene::GLTFTextureTransform;
    static constexpr auto value = object(
        "offset", &T::offset,
        "rotation", &T::rotation,
        "scale", &T::scale,
        "texCoord", &T::tex_coord
        );
};

template <>
struct glz::meta<kirana::scene::GLTFTextureInfo>
{
    using T = kirana::scene::GLTFTextureInfo;
    static constexpr auto value = object(
        "index", &T::index,
        "texCoord", &T::tex_coord,
        "extensions", glz::custom<&T::setExtensions, &T::getExtensions>
        );
};

template <>
struct glz::meta<kirana::scene::GLTFTextureInfoNormal>
{
    using T = kirana::scene::GLTFTextureInfoNormal;
    static constexpr auto value = object(
        "index", &T::index,
        "texCoord", &T::tex_coord,
        "scale", &T::scale,
        "extensions", glz::custom<&T::setExtensions, &T::getExtensions>
        );
};

template <>
struct glz::meta<kirana::scene::GLTFTextureInfoOcclusion>
{
    using T = kirana::scene::GLTFTextureInfoOcclusion;
    static constexpr auto value = object(
        "index", &T::index,
        "texCoord", &T::tex_coord,
        "strength", &T::strength,
        "extensions", glz::custom<&T::setExtensions, &T::getExtensions>
        );
};

template <>
struct glz::meta<kirana::scene::GLTFMaterialPBRMetallicRoughness>
{
    using T = kirana::scene::GLTFMaterialPBRMetallicRoughness;
    static constexpr auto value = object(
        "baseColorFactor", &T::base_color_factor,
        "baseColorTexture", &T::base_color_texture,
        "metallicFactor", &T::metallic_factor,
        "roughnessFactor", &T::roughness_factor,
        "metallicRoughnessTexture", &T::metallic_roughness_texture
        );
};

template <>
struct glz::meta<kirana::scene::GLTFMaterialAnisotropy>
{
    using T = kirana::scene::GLTFMaterialAnisotropy;
    static constexpr auto value = object(
        "anisotropyStrength", &T::strength,
        "anisotropyRotation", &T::rotation,
        "anisotropyTexture", &T::texture
        );
};

template <>
struct glz::meta<kirana::scene::GLTFMaterialClearcoat>
{
    using T = kirana::scene::GLTFMaterialClearcoat;
    static constexpr auto value = object(
        "clearcoatFactor", &T::factor,
        "clearcoatTexture", &T::texture,
        "clearcoatRoughnessFactor", &T::roughness_factor,
        "clearcoatRoughnessTexture", &T::roughness_texture,
        "clearcoatNormalTexture", &T::normal_texture
        );
};

template <>
struct glz::meta<kirana::scene::GLTFMaterialDiffuseTransmission>
{
    using T = kirana::scene::GLTFMaterialDiffuseTransmission;
    static constexpr auto value = object(
        "diffuseTransmissionFactor", &T::factor,
        "diffuseTransmissionTexture", &T::texture,
        "diffuseTransmissionColorFactor", &T::color_factor,
        "diffuseTransmissionColorTexture", &T::color_texture
        );
};

template <>
struct glz::meta<kirana::scene::GLTFMaterialDispersion>
{
    using T = kirana::scene::GLTFMaterialDispersion;
    static constexpr auto value = object(
        "dispersion", &T::dispersion
        );
};

template <>
struct glz::meta<kirana::scene::GLTFMaterialEmissiveStrength>
{
    using T = kirana::scene::GLTFMaterialEmissiveStrength;
    static constexpr auto value = object(
        "emissiveStrength", &T::strength
        );
};


template <>
struct glz::meta<kirana::scene::GLTFMaterialIOR>
{
    using T = kirana::scene::GLTFMaterialIOR;
    static constexpr auto value = object(
        "ior", &T::ior
        );
};

template <>
struct glz::meta<kirana::scene::GLTFMaterialIridescence>
{
    using T = kirana::scene::GLTFMaterialIridescence;
    static constexpr auto value = object(
        "iridescenceFactor", &T::factor,
        "iridescenceTexture", &T::texture,
        "iridescenceIor", &T::ior,
        "iridescenceThicknessMinimum", &T::thickness_min,
        "iridescenceThicknessMaximum", &T::thickness_max,
        "iridescenceThicknessTexture", &T::thickness_texture
        );
};

template <>
struct glz::meta<kirana::scene::GLTFMaterialSheen>
{
    using T = kirana::scene::GLTFMaterialSheen;
    static constexpr auto value = object(
        "sheenColorFactor", &T::color_factor,
        "sheenColorTexture", &T::color_texture,
        "sheenRoughnessFactor", &T::roughness_factor,
        "sheenRoughnessTexture", &T::roughness_texture
        );
};

template <>
struct glz::meta<kirana::scene::GLTFMaterialSpecular>
{
    using T = kirana::scene::GLTFMaterialSpecular;
    static constexpr auto value = object(
        "specularFactor", &T::factor,
        "specularTexture", &T::texture,
        "specularColorFactor", &T::color_factor,
        "specularColorTexture", &T::color_texture
        );
};

template <>
struct glz::meta<kirana::scene::GLTFMaterialTransmission>
{
    using T = kirana::scene::GLTFMaterialTransmission;
    static constexpr auto value = object(
        "transmissionFactor", &T::factor,
        "transmissionTexture", &T::texture
        );
};

template <>
struct glz::meta<kirana::scene::GLTFMaterialVolume>
{
    using T = kirana::scene::GLTFMaterialVolume;
    static constexpr auto value = object(
        "thicknessFactor", &T::thickness_factor,
        "thicknessTexture", &T::thickness_texture,
        "attenuationDistance", &T::attenuation_distance,
        "attenuationColor", &T::attenuation_color
        );
};

template <>
struct glz::meta<kirana::scene::GLTFMaterial>
{
    using T = kirana::scene::GLTFMaterial;
    static constexpr auto value = object(
        "name", &T::name,
        "pbrMetallicRoughness", &T::pbr_metallic_roughness,
        "normalTexture", &T::normal_texture,
        "occlusionTexture", &T::occlusion_texture,
        "emissiveTexture", &T::emissive_texture,
        "emissiveFactor", &T::emissive_factor,
        "alphaMode", &T::alpha_mode,
        "alphaCutoff", &T::alpha_cutoff,
        "doubleSided", &T::double_sided,
        "extensions", glz::custom<&T::setExtensions, &T::getExtensions>
        );
};

template <>
struct glz::meta<kirana::scene::GLTFMeshPrimitive>
{
    using T = kirana::scene::GLTFMeshPrimitive;
    static constexpr auto value = object(
        "attributes", &T::attributes,
        "indices", &T::indices,
        "material", &T::material,
        "mode", &T::mode,
        "targets", &T::targets
        );
};

template <>
struct glz::meta<kirana::scene::GLTFMesh>
{
    using T = kirana::scene::GLTFMesh;
    static constexpr auto value = object(
        "primitives", &T::primitives,
        "weights", &T::weights,
        "name", &T::name
        );
};

template <>
struct glz::meta<kirana::scene::GLTFCameraOrthographic>
{
    using T = kirana::scene::GLTFCameraOrthographic;
    static constexpr auto value = object(
        "xmag", &T::magnification_x,
        "ymag", &T::magnification_y,
        "zfar", &T::z_far,
        "znear", &T::z_near
        );
};

template <>
struct glz::meta<kirana::scene::GLTFCameraPerspective>
{
    using T = kirana::scene::GLTFCameraPerspective;
    static constexpr auto value = object(
        "aspectRatio", &T::aspect_ratio,
        "yfov", &T::fov_y,
        "zfar", &T::z_far,
        "znear", &T::z_near
        );
};

template <>
struct glz::meta<kirana::scene::GLTFCamera>
{
    using T = kirana::scene::GLTFCamera;
    static constexpr auto value = object(
        "orthographic", &T::orthographic,
        "perspective", &T::perspective,
        "type", &T::type,
        "name", &T::name
        );
};

template <>
struct glz::meta<kirana::scene::GLTFSpotLight>
{
    using T = kirana::scene::GLTFSpotLight;
    static constexpr auto value = object(
        "innerConeAngle", &T::cone_angle_inner,
        "outerConeAngle", &T::cone_angle_outer
        );
};

template <>
struct glz::meta<kirana::scene::GLTFPunctualLight>
{
    using T = kirana::scene::GLTFPunctualLight;
    static constexpr auto value = object(
        "type", &T::type,
        "color", &T::color,
        "intensity", &T::intensity,
        "range", &T::range,
        "spot", &T::spot,
        "name", &T::name
        );
};

template <>
struct glz::meta<kirana::scene::GLTFNode>
{
    using T = kirana::scene::GLTFNode;
    static constexpr auto value = object(
        "camera", &T::camera,
        "children", &T::children,
        "skin", &T::skin,
        "matrix", &T::matrix,
        "mesh", &T::mesh,
        "rotation", &T::rotation,
        "scale", &T::scale,
        "translation", &T::translation,
        "weights", &T::weights,
        "name", &T::name,
        "extensions", glz::custom<&T::setExtensions, &T::getExtensions>
        );
};

template <>
struct glz::meta<kirana::scene::GLTFSkin>
{
    using T = kirana::scene::GLTFSkin;
    static constexpr auto value = object(
        "inverseBindMatrices", &T::inverse_bind_matrices,
        "skeleton", &T::skeleton,
        "joints", &T::joints,
        "name", &T::name
        );
};

template <>
struct glz::meta<kirana::scene::GLTFScene>
{
    using T = kirana::scene::GLTFScene;
    static constexpr auto value = object(
        "nodes", &T::nodes,
        "name", &T::name
        );
};

template <>
struct glz::meta<kirana::scene::GLTFAnimationChannelTarget>
{
    using T = kirana::scene::GLTFAnimationChannelTarget;
    static constexpr auto value = object(
        "node", &T::node,
        "path", &T::path
        );
};

template <>
struct glz::meta<kirana::scene::GLTFAnimationChannel>
{
    using T = kirana::scene::GLTFAnimationChannel;
    static constexpr auto value = object(
        "sampler", &T::sampler,
        "target", &T::target
        );
};

template <>
struct glz::meta<kirana::scene::GLTFAnimationSampler>
{
    using T = kirana::scene::GLTFAnimationSampler;
    static constexpr auto value = object(
        "input", &T::input,
        "interpolation", &T::interpolation,
        "output", &T::output
        );
};

template <>
struct glz::meta<kirana::scene::GLTFAnimation>
{
    using T = kirana::scene::GLTFAnimation;
    static constexpr auto value = object(
        "channels", &T::channels,
        "samplers", &T::samplers,
        "name", &T::name
        );
};

template <>
struct glz::meta<kirana::scene::GLTFDocument>
{
    using T = kirana::scene::GLTFDocument;
    static constexpr auto value = object(
        "asset", &T::asset,
        "extensionsUsed", &T::extensions_used,
        "extensionsRequired", &T::extensions_required,
        "accessors", &T::accessors,
        "animations", &T::animations,
        "buffers", &T::buffers,
        "bufferViews", &T::buffer_views,
        "cameras", &T::cameras,
        "images", &T::images,
        "materials", &T::materials,
        "meshes", &T::meshes,
        "nodes", &T::nodes,
        "samplers", &T::samplers,
        "scene", &T::scene,
        "scenes", &T::scenes,
        "skins", &T::skins,
        "textures", &T::textures,
        "extensions", glz::custom<&T::setExtensions, &T::getExtensions>
        );
};
#pragma endregion
#endif //KIRANA_SCENE_GLTF_HPP