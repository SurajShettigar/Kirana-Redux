// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_COMMON_HPP
#define KIRANA_RENDERER_COMMON_HPP

#include <string>
#include <array>
#include <version_code.hpp>

namespace kirana::renderer
{
struct Size2D
{
    uint32_t width = 0;
    uint32_t height = 0;

    [[nodiscard]] bool isValid() const
    {
        return width != 0 && height != 0;
    }
};

struct Size3D
{
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t depth = 0;

    [[nodiscard]] bool isValid() const
    {
        return width != 0 && height != 0 && depth != 0;
    }
};

enum class TextureFormat
{
    UNKNOWN = 0,

    R8_UNORM = 1,
    R8_SNORM = 2,
    R8_USCALED = 3,
    R8_SSCALED = 4,
    R8_UINT = 5,
    R8_SINT = 6,
    R8_SRGB = 7,

    R8G8_UNORM = 8,
    R8G8_SNORM = 9,
    R8G8_USCALED = 10,
    R8G8_SSCALED = 11,
    R8G8_UINT = 12,
    R8G8_SINT = 13,
    R8G8_SRGB = 14,

    R8G8B8_UNORM = 15,
    R8G8B8_SNORM = 16,
    R8G8B8_USCALED = 17,
    R8G8B8_SSCALED = 18,
    R8G8B8_UINT = 19,
    R8G8B8_SINT = 20,
    R8G8B8_SRGB = 21,

    B8G8R8_UNORM = 22,
    B8G8R8_SNORM = 23,
    B8G8R8_USCALED = 24,
    B8G8R8_SSCALED = 25,
    B8G8R8_UINT = 26,
    B8G8R8_SINT = 27,
    B8G8R8_SRGB = 28,

    R8G8B8A8_UNORM = 29,
    R8G8B8A8_SNORM = 30,
    R8G8B8A8_USCALED = 31,
    R8G8B8A8_SSCALED = 32,
    R8G8B8A8_UINT = 33,
    R8G8B8A8_SINT = 34,
    R8G8B8A8_SRGB = 35,

    B8G8R8A8_UNORM = 36,
    B8G8R8A8_SNORM = 37,
    B8G8R8A8_USCALED = 38,
    B8G8R8A8_SSCALED = 39,
    B8G8R8A8_UINT = 40,
    B8G8R8A8_SINT = 41,
    B8G8R8A8_SRGB = 42,

    R16_UNORM = 43,
    R16_SNORM = 44,
    R16_USCALED = 45,
    R16_SSCALED = 46,
    R16_UINT = 47,
    R16_SINT = 48,
    R16_SFLOAT = 49,

    R16G16_UNORM = 50,
    R16G16_SNORM = 51,
    R16G16_USCALED = 52,
    R16G16_SSCALED = 53,
    R16G16_UINT = 54,
    R16G16_SINT = 55,
    R16G16_SFLOAT = 56,

    R16G16B16_UNORM = 57,
    R16G16B16_SNORM = 58,
    R16G16B16_USCALED = 59,
    R16G16B16_SSCALED = 60,
    R16G16B16_UINT = 61,
    R16G16B16_SINT = 62,
    R16G16B16_SFLOAT = 63,

    R16G16B16A16_UNORM = 64,
    R16G16B16A16_SNORM = 65,
    R16G16B16A16_USCALED = 66,
    R16G16B16A16_SSCALED = 67,
    R16G16B16A16_UINT = 68,
    R16G16B16A16_SINT = 69,
    R16G16B16A16_SFLOAT = 70,

    R32_UINT = 71,
    R32_SINT = 72,
    R32_SFLOAT = 73,

    R32G32_UINT = 74,
    R32G32_SINT = 75,
    R32G32_SFLOAT = 76,

    R32G32B32_UINT = 77,
    R32G32B32_SINT = 78,
    R32G32B32_SFLOAT = 79,

    R32G32B32A32_UINT = 80,
    R32G32B32A32_SINT = 81,
    R32G32B32A32_SFLOAT = 82,

    R64_UINT = 83,
    R64_SINT = 84,
    R64_SFLOAT = 85,

    R64G64_UINT = 86,
    R64G64_SINT = 87,
    R64G64_SFLOAT = 88,

    R64G64B64_UINT = 89,
    R64G64B64_SINT = 90,
    R64G64B64_SFLOAT = 91,

    R64G64B64A64_UINT = 92,
    R64G64B64A64_SINT = 93,
    R64G64B64A64_SFLOAT = 94,

    D16_UNORM = 95,
    D32_SFLOAT = 96,
    S8_UINT = 97,
    D16_UNORM_S8_UINT = 98,
    D24_UNORM_S8_UINT = 99,
    D32_SFLOAT_S8_UINT = 100,
};

enum class SampleCountFlags
{
    NONE = 0,
    S_1 = 1 << 0,
    S_2 = 1 << 1,
    S_4 = 1 << 2,
    S_8 = 1 << 3,
    S_16 = 1 << 4,
    S_32 = 1 << 5,
    S_64 = 1 << 6,
};

inline SampleCountFlags operator|(SampleCountFlags lhs, SampleCountFlags rhs)
{
    return static_cast<SampleCountFlags>(static_cast<uint32_t>(lhs) |
                                         static_cast<uint32_t>(rhs));
}

inline SampleCountFlags operator&(SampleCountFlags lhs, SampleCountFlags rhs)
{
    return static_cast<SampleCountFlags>(static_cast<uint32_t>(lhs) &
                                         static_cast<uint32_t>(rhs));
}

inline SampleCountFlags operator^(SampleCountFlags lhs, SampleCountFlags rhs)
{
    return static_cast<SampleCountFlags>(static_cast<uint32_t>(lhs) ^
                                         static_cast<uint32_t>(rhs));
}

inline SampleCountFlags operator~(SampleCountFlags flag)
{
    return static_cast<SampleCountFlags>(~static_cast<uint32_t>(flag));
}

inline bool hasFlag(const SampleCountFlags flags, const SampleCountFlags req_flag)
{
    return (flags & req_flag) == req_flag;
}

enum class SwapchainBufferMode
{
    UNKNOWN = 0,
    SINGLE = 1,
    DOUBLE = 2,
    TRIPLE = 3,
};

enum class PresentMode
{
    UNKNOWN = 0,
    IMMEDIATE = 1,
    MAILBOX = 2,
    FIFO = 3,
    FIFO_RELAXED = 4,
};

enum class GPUType
{
    UNKNOWN = 0,
    INTEGRATED = 1,
    DISCRETE = 2,
};

struct GPUFeatures
{
    // Base Features
    bool robust_buffer_access = false;
    bool full_draw_index_uint32 = false;
    bool image_cube_array = false;
    bool independent_blend = false;
    bool geometry_shader = false;
    bool tessellation_shader = false;
    bool sample_rate_shading = false;
    bool dual_src_blend = false;
    bool logic_op = false;
    bool multi_draw_indirect = false;
    bool draw_indirect_first_instance = false;
    bool depth_clamp = false;
    bool depth_bias_clamp = false;
    bool fill_mode_non_solid = false;
    bool depth_bounds = false;
    bool wide_lines = false;
    bool large_points = false;
    bool alpha_to_one = false;
    bool multi_viewport = false;
    bool sampler_anisotropy = false;
    bool texture_compression_ETC2 = false;
    bool texture_compression_ASTCLDR = false;
    bool texture_compression_BC = false;
    bool occlusion_query_precise = false;
    bool pipeline_statistics_query = false;
    bool vertex_pipeline_stores_and_atomics = false;
    bool fragment_stores_and_atomics = false;
    bool shader_tessellation_and_geometry_point_size = false;
    bool shader_image_gather_extended = false;
    bool shader_storage_image_extended_formats = false;
    bool shader_storage_image_multisample = false;
    bool shader_storage_image_read_without_format = false;
    bool shader_storage_image_write_without_format = false;
    bool shader_uniform_buffer_array_dynamic_indexing = false;
    bool shader_sampled_image_array_dynamic_indexing = false;
    bool shader_storage_buffer_array_dynamic_indexing = false;
    bool shader_storage_image_array_dynamic_indexing = false;
    bool shader_clip_distance = false;
    bool shader_cull_distance = false;
    bool shader_float64 = false;
    bool shader_int64 = false;
    bool shader_int16 = false;
    bool shader_resource_residency = false;
    bool shader_resource_min_lod = false;
    bool sparse_binding = false;
    bool sparse_residency_buffer = false;
    bool sparse_residency_image2D = false;
    bool sparse_residency_image3D = false;
    bool sparse_residency2_samples = false;
    bool sparse_residency4_samples = false;
    bool sparse_residency8_samples = false;
    bool sparse_residency16_samples = false;
    bool sparse_residency_aliased = false;
    bool variable_multisample_rate = false;
    bool inherited_queries = false;
    // 16-bit float 8-bit int features
    bool shader_float16 = false;
    bool shader_int8 = false;
    // int 64-bit atomic features
    bool shader_buffer_int64_atomics = false;
    bool shader_shared_int64_atomics = false;
    // Bindless support features
    bool shader_input_attachment_array_dynamic_indexing = false;
    bool shader_uniform_texel_buffer_array_dynamic_indexing = false;
    bool shader_storage_texel_buffer_array_dynamic_indexing = false;
    bool shader_uniform_buffer_array_non_uniform_indexing = false;
    bool shader_sampled_image_array_non_uniform_indexing = false;
    bool shader_storage_buffer_array_non_uniform_indexing = false;
    bool shader_storage_image_array_non_uniform_indexing = false;
    bool shader_input_attachment_array_non_uniform_indexing = false;
    bool shader_uniform_texel_buffer_array_non_uniform_indexing = false;
    bool shader_storage_texel_buffer_array_non_uniform_indexing = false;
    bool descriptor_binding_uniform_buffer_update_after_bind = false;
    bool descriptor_binding_sampled_image_update_after_bind = false;
    bool descriptor_binding_storage_image_update_after_bind = false;
    bool descriptor_binding_storage_buffer_update_after_bind = false;
    bool descriptor_binding_uniform_texel_buffer_update_after_bind = false;
    bool descriptor_binding_storage_texel_buffer_update_after_bind = false;
    bool descriptor_binding_update_unused_while_pending = false;
    bool descriptor_binding_partially_bound = false;
    bool descriptor_binding_variable_descriptor_count = false;
    bool runtime_descriptor_array = false;
    // Acceleration Structure features
    bool acceleration_structure = false;
    bool acceleration_structure_capture_replay = false;
    bool acceleration_structure_indirect_build = false;
    bool acceleration_structure_host_commands = false;
    bool descriptor_binding_acceleration_structure_update_after_bind = false;
    // Raytracing features
    bool ray_tracing_pipeline = false;
    bool ray_tracing_pipeline_shader_group_handle_capture_replay = false;
    bool ray_tracing_pipeline_shader_group_handle_capture_replay_mixed = false;
    bool ray_tracing_pipeline_trace_rays_indirect = false;
    bool ray_traversal_primitive_culling = false;
};

struct GPULimits
{
    uint32_t max_image_dimension_1D = 0;
    uint32_t max_image_dimension_2D = 0;
    uint32_t max_image_dimension_3D = 0;
    uint32_t max_image_dimension_cube = 0;
    uint32_t max_image_array_layers = 0;
    uint32_t max_texel_buffer_elements = 0;
    uint32_t max_uniform_buffer_range = 0;
    uint32_t max_storage_buffer_range = 0;
    uint32_t max_push_constants_size = 0;
    uint32_t max_memory_allocation_count = 0;
    uint32_t max_sampler_allocation_count = 0;
    uint64_t buffer_image_granularity = 0;
    uint64_t sparse_address_space_size = 0;
    uint32_t max_bound_descriptor_sets = 0;
    uint32_t max_per_stage_descriptor_samplers = 0;
    uint32_t max_per_stage_descriptor_uniform_buffers = 0;
    uint32_t max_per_stage_descriptor_storage_buffers = 0;
    uint32_t max_per_stage_descriptor_sampled_images = 0;
    uint32_t max_per_stage_descriptor_storage_images = 0;
    uint32_t max_per_stage_descriptor_input_attachments = 0;
    uint32_t max_per_stage_resources = 0;
    uint32_t max_descriptor_set_samplers = 0;
    uint32_t max_descriptor_set_uniform_buffers = 0;
    uint32_t max_descriptor_set_uniform_buffers_dynamic = 0;
    uint32_t max_descriptor_set_storage_buffers = 0;
    uint32_t max_descriptor_set_storage_buffers_dynamic = 0;
    uint32_t max_descriptor_set_sampled_images = 0;
    uint32_t max_descriptor_set_storage_images = 0;
    uint32_t max_descriptor_set_input_attachments = 0;
    uint32_t max_vertex_input_attributes = 0;
    uint32_t max_vertex_input_bindings = 0;
    uint32_t max_vertex_input_attribute_offset = 0;
    uint32_t max_vertex_input_binding_stride = 0;
    uint32_t max_vertex_output_components = 0;
    uint32_t max_tessellation_generation_level = 0;
    uint32_t max_tessellation_patch_size = 0;
    uint32_t max_tessellation_control_per_vertex_input_components = 0;
    uint32_t max_tessellation_control_per_vertex_output_components = 0;
    uint32_t max_tessellation_control_per_patch_output_components = 0;
    uint32_t max_tessellation_control_total_output_components = 0;
    uint32_t max_tessellation_evaluation_input_components = 0;
    uint32_t max_tessellation_evaluation_output_components = 0;
    uint32_t max_geometry_shader_invocations = 0;
    uint32_t max_geometry_input_components = 0;
    uint32_t max_geometry_output_components = 0;
    uint32_t max_geometry_output_vertices = 0;
    uint32_t max_geometry_total_output_components = 0;
    uint32_t max_fragment_input_components = 0;
    uint32_t max_fragment_output_attachments = 0;
    uint32_t max_fragment_dual_src_attachments = 0;
    uint32_t max_fragment_combined_output_resources = 0;
    uint32_t max_compute_shared_memory_size = 0;
    std::array<uint32_t, 3> max_compute_work_group_count{0, 0, 0};
    uint32_t max_compute_work_group_invocations = 0;
    std::array<uint32_t, 3> max_compute_work_group_size{0, 0, 0};
    uint32_t sub_pixel_precision_bits = 0;
    uint32_t sub_texel_precision_bits = 0;
    uint32_t mipmap_precision_bits = 0;
    uint32_t max_draw_indexed_index_value = 0;
    uint32_t max_draw_indirect_count = 0;
    float max_sampler_lod_bias = 0.0f;
    float max_sampler_anisotropy = 0.0f;
    uint32_t max_viewports = 0;
    std::array<uint32_t, 2> max_viewport_dimensions{0, 0};
    std::array<float, 2> viewport_bounds_range{0.0f, 0.0f};
    uint32_t viewport_sub_pixel_bits = 0;
    size_t min_memory_map_alignment = 0;
    uint64_t min_texel_buffer_offset_alignment = 0;
    uint64_t min_uniform_buffer_offset_alignment = 0;
    uint64_t min_storage_buffer_offset_alignment = 0;
    int32_t min_texel_offset = 0;
    uint32_t max_texel_offset = 0;
    int32_t min_texel_gather_offset = 0;
    uint32_t max_texel_gather_offset = 0;
    float min_interpolation_offset = 0.0f;
    float max_interpolation_offset = 0.0f;
    uint32_t sub_pixel_interpolation_offset_bits = 0;
    uint32_t max_framebuffer_width = 0;
    uint32_t max_framebuffer_height = 0;
    uint32_t max_framebuffer_layers = 0;
    SampleCountFlags framebuffer_color_sample_counts = SampleCountFlags::NONE;
    SampleCountFlags framebuffer_depth_sample_counts = SampleCountFlags::NONE;
    SampleCountFlags framebuffer_stencil_sample_counts = SampleCountFlags::NONE;
    SampleCountFlags framebuffer_no_attachments_sample_counts =
        SampleCountFlags::NONE;
    uint32_t max_color_attachments = 0;
    SampleCountFlags sampled_image_color_sample_counts = SampleCountFlags::NONE;
    SampleCountFlags sampled_image_integer_sample_counts = SampleCountFlags::NONE;
    SampleCountFlags sampled_image_depth_sample_counts = SampleCountFlags::NONE;
    SampleCountFlags sampled_image_stencil_sample_counts = SampleCountFlags::NONE;
    SampleCountFlags storage_image_sample_counts = SampleCountFlags::NONE;
    uint32_t max_sample_mask_words = 0;
    bool timestamp_compute_and_graphics = false;
    float timestamp_period = 0.0f;
    uint32_t max_clip_distances = 0;
    uint32_t max_cull_distances = 0;
    uint32_t max_combined_clip_and_cull_distances = 0;
    uint32_t discrete_queue_priorities = 0;
    std::array<float, 2> point_size_range{0.0f, 0.0f};
    std::array<float, 2> line_width_range{0.0f, 0.0f};
    float point_size_granularity = 0.0f;
    float line_width_granularity = 0.0f;
    bool strict_lines = false;
    bool standard_sample_locations = false;
    uint64_t optimal_buffer_copy_offset_alignment = 0;
    uint64_t optimal_buffer_copy_row_pitch_alignment = 0;
    uint64_t non_coherent_atom_size = 0;
    // bindless limits
    uint32_t max_update_after_bind_descriptors_in_all_pools = 0;
    bool shader_uniform_buffer_array_non_uniform_indexing_native = false;
    bool shader_sampled_image_array_non_uniform_indexing_native = false;
    bool shader_storage_buffer_array_non_uniform_indexing_native = false;
    bool shader_storage_image_array_non_uniform_indexing_native = false;
    bool shader_input_attachment_array_non_uniform_indexing_native = false;
    bool robust_buffer_access_update_after_bind = false;
    bool quad_divergent_implicit_lod = false;
    uint32_t max_per_stage_descriptor_update_after_bind_samplers = 0;
    uint32_t max_per_stage_descriptor_update_after_bind_uniform_buffers = 0;
    uint32_t max_per_stage_descriptor_update_after_bind_storage_buffers = 0;
    uint32_t max_per_stage_descriptor_update_after_bind_sampled_images = 0;
    uint32_t max_per_stage_descriptor_update_after_bind_storage_images = 0;
    uint32_t max_per_stage_descriptor_update_after_bind_input_attachments = 0;
    uint32_t max_per_stage_update_after_bind_resources = 0;
    uint32_t max_descriptor_set_update_after_bind_samplers = 0;
    uint32_t max_descriptor_set_update_after_bind_uniform_buffers = 0;
    uint32_t max_descriptor_set_update_after_bind_uniform_buffers_dynamic = 0;
    uint32_t max_descriptor_set_update_after_bind_storage_buffers = 0;
    uint32_t max_descriptor_set_update_after_bind_storage_buffers_dynamic = 0;
    uint32_t max_descriptor_set_update_after_bind_sampled_images = 0;
    uint32_t max_descriptor_set_update_after_bind_storage_images = 0;
    uint32_t max_descriptor_set_update_after_bind_input_attachments = 0;
    // Acceleration Structure limits
    uint64_t max_geometry_count = 0;
    uint64_t max_instance_count = 0;
    uint64_t max_primitive_count = 0;
    uint32_t max_per_stage_descriptor_acceleration_structures = 0;
    uint32_t max_per_stage_descriptor_update_after_bind_acceleration_structures =
        0;
    uint32_t max_descriptor_set_acceleration_structures = 0;
    uint32_t max_descriptor_set_update_after_bind_acceleration_structures = 0;
    uint32_t min_acceleration_structure_scratch_offset_alignment = 0;
    // Raytracing limits
    uint32_t shader_group_handle_size = 0;
    uint32_t max_ray_recursion_depth = 0;
    uint32_t max_shader_group_stride = 0;
    uint32_t shader_group_base_alignment = 0;
    uint32_t shader_group_handle_capture_replay_size = 0;
    uint32_t max_ray_dispatch_invocation_count = 0;
    uint32_t shader_group_handle_alignment = 0;
    uint32_t max_ray_hit_attribute_size = 0;
};


enum class QueueFamilyFlags
{
    NONE = 0,
    GRAPHICS = 1 << 0,
    PRESENTATION = 1 << 1,
    COMPUTE = 1 << 2,
    TRANSFER = 1 << 3,
};

inline QueueFamilyFlags operator|(QueueFamilyFlags lhs, QueueFamilyFlags rhs)
{
    return static_cast<QueueFamilyFlags>(static_cast<uint32_t>(lhs) |
                                         static_cast<uint32_t>(rhs));
}

inline QueueFamilyFlags operator&(QueueFamilyFlags lhs, QueueFamilyFlags rhs)
{
    return static_cast<QueueFamilyFlags>(static_cast<uint32_t>(lhs) &
                                         static_cast<uint32_t>(rhs));
}

inline QueueFamilyFlags operator^(QueueFamilyFlags lhs, QueueFamilyFlags rhs)
{
    return static_cast<QueueFamilyFlags>(static_cast<uint32_t>(lhs) ^
                                         static_cast<uint32_t>(rhs));
}

inline QueueFamilyFlags operator~(QueueFamilyFlags flag)
{
    return static_cast<QueueFamilyFlags>(~static_cast<uint32_t>(flag));
}

inline bool hasFlag(const QueueFamilyFlags flags, const QueueFamilyFlags req_flag)
{
    return (flags & req_flag) == req_flag;
}

struct QueueFamily
{
    uint32_t index = std::numeric_limits<uint32_t>::max();
    uint32_t num_queues = 0;
    QueueFamilyFlags support_flags = QueueFamilyFlags::NONE;

    [[nodiscard]] bool isValid() const
    {
        return index < std::numeric_limits<uint32_t>::max() && num_queues > 0 &&
               support_flags != QueueFamilyFlags::NONE;
    }

    [[nodiscard]] bool supportsRendering() const
    {
        return hasFlag(support_flags, QueueFamilyFlags::GRAPHICS);
    }

    [[nodiscard]] bool supportsPresentation() const
    {
        return hasFlag(support_flags, QueueFamilyFlags::PRESENTATION);
    }

    [[nodiscard]] bool supportsCompute() const
    {
        return hasFlag(support_flags, QueueFamilyFlags::COMPUTE);
    }

    [[nodiscard]] bool supportsTransfer() const
    {
        return hasFlag(support_flags, QueueFamilyFlags::TRANSFER);
    }

    [[nodiscard]] bool supportsSurfaceRendering() const
    {
        return supportsRendering() && supportsPresentation();
    }

    [[nodiscard]] bool supportsOnlyCompute() const
    {
        return supportsCompute() && !supportsRendering();
    }

    [[nodiscard]] bool supportsOnlyTransfer() const
    {
        return supportsTransfer() &&
               !(supportsCompute() || supportsRendering());
    }
};

struct Queue
{
    uint32_t index = std::numeric_limits<uint32_t>::max();
    uint32_t family_index = std::numeric_limits<uint32_t>::max();
    QueueFamilyFlags type = QueueFamilyFlags::NONE;

    [[nodiscard]] bool isValid() const
    {
        return index < std::numeric_limits<uint32_t>::max() && family_index < std::numeric_limits<uint32_t>::max()
               && type != QueueFamilyFlags::NONE;
    }
};

struct SurfaceData
{
    /// HWND for Windows API, Window for X-LIB API.
    void *window_handle{nullptr};
    /// HINSTANCE for Windows API, Display for X-LIB API.
    void *instance_handle{nullptr};

    [[nodiscard]] bool isValid() const
    {
        return window_handle != nullptr && instance_handle != nullptr;
    }
};

struct GPUSelectionPreference
{
    GPUType type = GPUType::UNKNOWN;
    GPUFeatures features = {};
    GPULimits limits = {};
    QueueFamilyFlags queue_types = QueueFamilyFlags::NONE;
    bool dedicated_compute_queue = false;
    bool dedicated_transfer_queue = false;
};

struct DeviceInitializationData
{
    bool debug_mode = {false};
    std::string app_name = {};
    core::VersionCode version = {};
    SurfaceData surface = {};
    GPUSelectionPreference gpu_preference = {};
};

struct SwapchainData
{
    Size2D size{};
    TextureFormat format{TextureFormat::UNKNOWN};
    SwapchainBufferMode buffer_mode{SwapchainBufferMode::DOUBLE};
    PresentMode present_mode{PresentMode::IMMEDIATE};
};

}
#endif // KIRANA_RENDERER_COMMON_HPP