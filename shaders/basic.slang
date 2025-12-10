import common;

struct VertexInput
{
    uint index : SV_VertexID;
    uint instance_base_offset : SV_StartInstanceLocation;
    uint instance_index : SV_InstanceID;
}

struct VertexOutput
{
    float4 position : SV_Position;
    float3 world_position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal: NORMAL;
    nointerpolation uint mat_id;
}

struct FragmentOutput
{
    float4 color : SV_Target;
}

ConstantBuffer<Camera> camera;
StructuredBuffer<float4> positions;
StructuredBuffer<float4> normals;
StructuredBuffer<float4> uvs;
StructuredBuffer<Mesh> meshes;
StructuredBuffer<Transform> transforms;
StructuredBuffer<Instance> mesh_instances;
StructuredBuffer<TextureData> textures_data;
StructuredBuffer<MaterialPBR> materials;

SamplerState samplers[];
Texture2D<float4> textures[];


float4 sampleTexture(uint index, float2 uv) {
    if(index == INVALID_ID) {
        return float4(0.0);
    }

    const TextureData data = textures_data[index];
    return textures[data.t_index].SampleLevel(samplers[data.s_index], uv, 0.0);
}

[shader("vertex")]
VertexOutput mainVS(VertexInput input)
{
    const uint instance_index = input.instance_base_offset + input.instance_index;
    const Instance instance = mesh_instances[instance_index];
    const Mesh mesh = meshes[instance.d_index];

    const float4 pos = float4(positions[mesh.pos_offset + input.index].xyz, 1.0);
    const float4 norm = float4(normals[mesh.normal_offset + input.index].xyz, 0.0);
    const float2 uv = uvs[mesh.uv_offset + input.index].xy;

    const float4 world_position = mul(pos, transforms[instance.t_index].world);
    const float4 clip_pos = mul(world_position, mul(camera.view_matrix, camera.proj_matrix));

    const float3 world_normal = mul(norm, transpose(transforms[instance.t_index].local)).xyz;

    return VertexOutput(clip_pos, world_position.xyz, uv, world_normal, mesh.material_index);
}

[shader("fragment")]
FragmentOutput mainFS(VertexOutput v_input)
{
    const MaterialPBR mat = materials[v_input.mat_id];
    const float4 color = mat.base_color * sampleTexture(mat.texture_base_color_index, v_input.uv);
    return FragmentOutput(float4(color.rgb, 1.0));
}