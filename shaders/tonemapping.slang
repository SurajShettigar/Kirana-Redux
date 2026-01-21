import common;

RWTexture2D<float4> render;

/// Converts a color from linear RGB to sRGB.
inline float3 toSRGB(float3 color)
{
  float3 low  = color * 12.92f;
  float3 high = fma(pow(color, float3(0.41666)), float3(1.055), float3(-0.055));
  return lerp(low, high, float3(color > float3(0.0031308)));
}

/// Converts a color from sRGB to linear RGB.
inline float3 toLinear(float3 color)
{
  float3 low  = color / 12.92;
  float3 high = pow((color + float3(0.055)) / 1.055F, float3(2.4));
  return lerp(low, high, float3(color > float3(0.04045)));
}

/// Applies Khronos PBR Netural Tonemapping to the given input color (in linear space) and returns 
/// a tonemapped color in sRGB color space.
/// Reference: https://github.com/KhronosGroup/ToneMapping/tree/main/PBR_Neutral
/// Explanation: https://modelviewer.dev/examples/tone-mapping
float3 khronosPBRNeturalTonemapper(float3 color) {
    const float start_compression = 0.8 - 0.04;
    
    float x = min(color.r, min(color.g, color.b));
    float offset = x < 0.08 ? x - (6.25 * x * x): 0.04;
    color -= offset;

    float peak = max(color.r, max(color.g, color.b));
    if (peak < start_compression) {
        return toSRGB(color);
    }

    const float desaturation = 0.15;
    const float d = 1.0 - start_compression;

    float new_peak = 1.0 - d * d / (peak + d - start_compression);
    color *= new_peak / peak;
    
    float g = 1.0 - 1.0 / (desaturation * (peak - new_peak) + 1.0);

    return toSRGB(lerp(color, float3(new_peak), g));
}

[shader("compute")]
[numthreads(16,16,1)]
void main(uint3 g_id : SV_DispatchThreadID)
{
    uint2 texel_coords = g_id.xy;
    uint2 size = uint2(0, 0);
    render.GetDimensions(size.x, size.y);

    if (texel_coords.x >= size.x || texel_coords.y >= size.y) {
        return;
    }
    
    float4 color = render.Load(int2(texel_coords));

    color.rgb = khronosPBRNeturalTonemapper(color.rgb);

    render.Store(texel_coords, color);
}