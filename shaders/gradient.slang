RWTexture2D<float4> result;

[shader("compute")]
[numthreads(16,16,1)]
void main(uint3 g_id : SV_DispatchThreadID)
{
    uint2 texel_coords = g_id.xy;
    uint2 size = uint2(0, 0);
    result.GetDimensions(size.x, size.y);

    if (texel_coords.x >= size.x || texel_coords.y >= size.y) {
        return;
    }

    float4 color = float4(float3(0.0), 1.0);
    color.x = float(texel_coords.x) / float(size.x);
    color.y = float(texel_coords.y) / float(size.y);

    result.Store(texel_coords, color);
}