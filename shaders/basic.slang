struct VertexInput {
    uint index: SV_VertexID;
}

struct VertexOutput {
    float4 position : SV_Position;
    float4 color;
}

struct FragmentOutput {
    float4 color : SV_Target;
}

StructuredBuffer<float4> positions;
StructuredBuffer<float4> colors;

[shader("vertex")]
VertexOutput vertexMain(VertexInput input) {
    return VertexOutput(positions[input.index], colors[input.index]);
}

[shader("fragment")]
FragmentOutput fragmentMain(VertexOutput v_input) {
    return FragmentOutput(v_input.color);
}