import common;

struct VertexOutput
{
    float4 position : SV_Position;
    float3 world_position : POSITION;
    float2 uv : TEXCOORD;
}

struct FragmentOutput
{
    float4 color : SV_Target;
    float depth: SV_Depth;
}

ConstantBuffer<Camera> camera;
ConstantBuffer<EnvironmentLight> light;
Sampler2D<float4> tex;

float2 sphericalUV(in float3 dir) {
    float gamma = asin(dir.y);
    float theta = atan2(dir.z, dir.x);
    return float2(theta * ONE_OVER_PI * 0.5, gamma * ONE_OVER_PI) + 0.5;
}

[shader("vertex")]
VertexOutput mainVS(uint index: SV_VertexID)
{
    float2 uv = float2((index << 1) & 2, index & 2);
    const float4 pos = float4(uv * 2.0 - float2(1.0), 1.0, 1.0);
    return VertexOutput(pos, pos.xyz, uv);
}

[shader("fragment")]
FragmentOutput mainFS(VertexOutput v_input)
{
    float3 dir = normalize(v_input.world_position);
    dir = mul(float4(dir, 0.0), light.rotation_matrix).xyz;
    const float2 uv = sphericalUV(dir);
    return FragmentOutput(tex.SampleLevel(uv, 0.0), 1.0);
}