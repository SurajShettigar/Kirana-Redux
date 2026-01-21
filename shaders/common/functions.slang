implementing common;

/// Maps x value from 0-in_max to 0-out_max range.
public uint map0(uint x, uint in_max, uint out_max) {
    return (x * out_max) / in_max;
}

/// Maps x value from 0-in_max to 0-out_max range.
public uint2 map0(uint2 x, uint2 in_max, uint2 out_max) {
    return (x * out_max) / in_max;
}

/// Maps x value from in_min-in_max to out_min-out_max range.
public uint map(uint x, uint2 in_range, uint2 out_range) {
    return out_range.x + ((x - in_range.x) * (out_range.y - out_range.x)) / (in_range.y - in_range.x);
}

/// Maps x value from in_min-in_max to out_min-out_max range.
public float map(uint x, float2 in_range, float2 out_range) {
    return out_range.x + ((x - in_range.x) * (out_range.y - out_range.x)) / (in_range.y - in_range.x);
}