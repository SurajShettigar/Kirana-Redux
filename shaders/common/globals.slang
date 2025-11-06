implementing common;

public enum IndexFormat : uint {
    UINT = 0u,
    UINT_16 = 1u,
    UINT_8 = 2u,
}

public struct Transform {
    public float4x4 world;
    public float4x4 local;
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

public struct MeshInstance {
    public uint mesh;
    public uint transform;
}

public struct Camera {
    public float4x4 view_matrix;
    public float4x4 proj_matrix;
}