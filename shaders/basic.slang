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
}

struct FragmentOutput
{
    float4 color : SV_Target;
}

StructuredBuffer<float4> positions;
StructuredBuffer<float4> normals;
StructuredBuffer<float2> uvs;
StructuredBuffer<Mesh> meshes;
StructuredBuffer<Transform> transforms;
StructuredBuffer<MeshInstance> mesh_instances;
ConstantBuffer<Camera> camera;

[shader("vertex")]
VertexOutput mainVS(VertexInput input)
{
    const uint instance_index = input.instance_base_offset + input.instance_index;
    const MeshInstance instance = mesh_instances[instance_index];
    const Mesh mesh = meshes[instance.mesh];

    const float4 pos = float4(positions[mesh.pos_offset + input.index].xyz, 1.0);
    const float4 norm = float4(normals[mesh.normal_offset + input.index].xyz, 0.0);
    const float2 uv = uvs[mesh.uv_offset + input.index];

    const float4 world_position = mul(pos, transforms[instance.transform].world);
    const float4 clip_pos = mul(world_position, mul(camera.view_matrix, camera.proj_matrix));

    const float3 world_normal = mul(norm, transpose(transforms[instance.transform].local)).xyz;

    return VertexOutput(clip_pos, world_position.xyz, uv, world_normal);
}

[shader("fragment")]
FragmentOutput mainFS(VertexOutput v_input)
{
    return FragmentOutput(float4(v_input.normal, 1.0));
}