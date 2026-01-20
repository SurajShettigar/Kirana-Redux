implementing common;

public static const uint INVALID_ID = 4294967295u;

public static const float PI = 3.1415926535897932384626433832795;
public static const float TWO_PI = 6.283185307179586476925286766559;
public static const float FOUR_PI = 12.566370614359172953850573533118;
public static const float ONE_OVER_PI = 0.31830988618379067153776752674503;
public static const float ONE_OVER_TWO_PI = 0.15915494309189533576888376337251;
public static const float ONE_OVER_FOUR_PI = 0.07957747154594766788444188168626;

public struct TextureData {
    public float2 offset;
    public float2 scale;
    public float rotation;
    public uint uv_index;
    public uint t_index;
    public uint s_index;
}

public struct Transform {
    public float4x4 world;
    public float4x4 local;
}

public enum LightUnit: uint {
    /// Luminous FLux (lm) Radiometric Unit => Radiant Flux (watt (W))
    LUMEN = 0u,
    /// Luminous Intensity (lm/sr = cd) Radiometric Unit => Intensity (W/sr)
    CANDELA = 1u,
    /// Illuminance (lm/m² = lx) Radiometric Unit => Irradiance (W/m²)
    LUX = 2u,
    /// Luminance (lm/m²sr = cd/m²) Radiometric Unit => Radiance (W/m²sr)
    NIT = 2u,
}

public enum PunctualLightType: uint {
    DIRECTIONAL = 0u,
    POINT = 1u,
    SPOT = 2u,
}

public struct EnvironmentLight {
    public float3 color;
    public float intensity;

    public float4x4 rotation_matrix;

    public LightUnit unit;
    public uint texture_index;
}

public struct PunctualLight {
    public float3 color;
    public PunctualLightType type;
    public float intensity;
    public float range;
    public float spot_cone_angle_outer;
    public float spot_cone_angle_inner;
    float3 _padding;
    public LightUnit unit;
}

public enum AlphaMode: uint {
    OPAQUE = 0u,
    MASK = 1u,
    BLEND = 2u,
}

public struct MaterialPBR
{
    public float4 base_color;

    public float3 specular_color;
    public float specular_factor;

    public float3 emissive_color;
    public float emissive_strength;

    public float3 sheen_color;
    public float sheen_roughness_factor;

    public float metallic_factor;
    public float roughness_factor;
    public float anisotropy_strength;
    public float anisotropy_rotation;

    public float iridescence_factor;
    public float iridescence_ior;
    public float iridescence_thickness_min;
    public float iridescence_thickness_max;

    public float3 diffuse_transmission_color;
    public float diffuse_transmission_factor;

    public float3 volume_attenuation_color;
    public float volume_thickness_factor;

    public float volume_attenuation_distance;
    public float dispersion_factor;
    public float transmission_factor;
    public float ior;

    public float clearcoat_factor;
    public float clearcoat_roughness_factor;
    public float normal_scale;
    public float occlusion_strength;

    public uint texture_base_color_index;
    public uint texture_specular_color_index;
    /// A - Specular factor.
    public uint texture_specular_index;
    public uint texture_emissive_color_index;

    public uint texture_sheen_color_index;
    /// A - Sheen roughness factor.
    public uint texture_sheen_roughness_index;
    /// R - Undefined, G - Roughness, B - Metallic
    public uint texture_metallic_roughness_index;
    /// RG - Tangent, Bitangent rotation direction (Convert [0, 1] to -[1, 1] range). B - Anisotropy strength.
    public uint texture_anisotropy_index;

    /// R - Iridescence factor.
    public uint texture_iridescence_index;
    /// G - Iridescence thickness. Value is used to lerp from min-max thickness values.
    public uint texture_iridescence_thickness_index;
    public uint texture_diffuse_transmission_color_index;
    /// A - Diffuse transmission factor.
    public uint texture_diffuse_transmission_index;

    /// G - Volume thickness factor.
    public uint texture_volume_thickness_index;
    public uint texture_transmission_index;
    /// R - Clearcoat strength.
    public uint texture_clearcoat_index;
    /// G - Clearcoat roughness.
    public uint texture_clearcoat_roughness_index;

    public uint texture_clearcoat_normal_index;
    public uint texture_normal_index;
    public uint texture_occlusion_index;
    public AlphaMode alpha_mode;

    public float alpha_cutoff;
    public bool double_sided;
    uint2 _padding;
};

public enum IndexFormat : uint {
    UINT = 0u,
    UINT_16 = 1u,
    UINT_8 = 2u,
}

public struct Mesh {
    public IndexFormat index_format;
    public uint index_offset;
    public uint index_count;
    public uint pos_offset;
    public uint pos_count;
    public uint normal_offset;
    public uint normal_count;
    public uint uv_offset;
    public uint uv_count;
    public uint color_offset;
    public uint color_count;
    public uint material_index;
}

public struct Instance {
    public uint d_index;
    public uint t_index;
}

public struct Camera {
    public float4x4 view_matrix;
    public float4x4 proj_matrix;
}