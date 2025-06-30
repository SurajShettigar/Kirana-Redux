// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_HELPERS_VULKAN_HPP
#define KIRANA_RENDERER_HELPERS_VULKAN_HPP

#include <vulkan/vulkan.hpp>

#include "common.hpp"

namespace kirana::renderer
{
const auto LOG_CHANNEL_VULKAN = "RENDERER_VULKAN";

/// Timeout in nanoseconds.
constexpr uint32_t FENCE_WAIT_TIMEOUT = 1000000000; // 1 second
constexpr uint32_t SWAPCHAIN_FETCH_TIMEOUT = 1000000000; // 1 second

inline vk::Extent2D getExtent2D(const Size2D &size)
{
    return vk::Extent2D{size.width, size.height};
}

inline vk::Format getFormat(const TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::R8_UNORM:
        return vk::Format::eR8Unorm;
    case TextureFormat::R8_SNORM:
        return vk::Format::eR8Snorm;
    case TextureFormat::R8_USCALED:
        return vk::Format::eR8Uscaled;
    case TextureFormat::R8_SSCALED:
        return vk::Format::eR8Sscaled;
    case TextureFormat::R8_UINT:
        return vk::Format::eR8Uint;
    case TextureFormat::R8_SINT:
        return vk::Format::eR8Sint;
    case TextureFormat::R8_SRGB:
        return vk::Format::eR8Srgb;

    case TextureFormat::R8G8_UNORM:
        return vk::Format::eR8G8Unorm;
    case TextureFormat::R8G8_SNORM:
        return vk::Format::eR8G8Snorm;
    case TextureFormat::R8G8_USCALED:
        return vk::Format::eR8G8Uscaled;
    case TextureFormat::R8G8_SSCALED:
        return vk::Format::eR8G8Sscaled;
    case TextureFormat::R8G8_UINT:
        return vk::Format::eR8G8Uint;
    case TextureFormat::R8G8_SINT:
        return vk::Format::eR8G8Sint;
    case TextureFormat::R8G8_SRGB:
        return vk::Format::eR8G8Srgb;

    case TextureFormat::R8G8B8_UNORM:
        return vk::Format::eR8G8B8Unorm;
    case TextureFormat::R8G8B8_SNORM:
        return vk::Format::eR8G8B8Snorm;
    case TextureFormat::R8G8B8_USCALED:
        return vk::Format::eR8G8B8Uscaled;
    case TextureFormat::R8G8B8_SSCALED:
        return vk::Format::eR8G8B8Sscaled;
    case TextureFormat::R8G8B8_UINT:
        return vk::Format::eR8G8B8Uint;
    case TextureFormat::R8G8B8_SINT:
        return vk::Format::eR8G8B8Sint;
    case TextureFormat::R8G8B8_SRGB:
        return vk::Format::eR8G8B8Srgb;

    case TextureFormat::B8G8R8_UNORM:
        return vk::Format::eB8G8R8Unorm;
    case TextureFormat::B8G8R8_SNORM:
        return vk::Format::eB8G8R8Snorm;
    case TextureFormat::B8G8R8_USCALED:
        return vk::Format::eB8G8R8Uscaled;
    case TextureFormat::B8G8R8_SSCALED:
        return vk::Format::eB8G8R8Sscaled;
    case TextureFormat::B8G8R8_UINT:
        return vk::Format::eB8G8R8Uint;
    case TextureFormat::B8G8R8_SINT:
        return vk::Format::eB8G8R8Sint;
    case TextureFormat::B8G8R8_SRGB:
        return vk::Format::eB8G8R8Srgb;

    case TextureFormat::R8G8B8A8_UNORM:
        return vk::Format::eR8G8B8A8Unorm;
    case TextureFormat::R8G8B8A8_SNORM:
        return vk::Format::eR8G8B8A8Snorm;
    case TextureFormat::R8G8B8A8_USCALED:
        return vk::Format::eR8G8B8A8Uscaled;
    case TextureFormat::R8G8B8A8_SSCALED:
        return vk::Format::eR8G8B8A8Sscaled;
    case TextureFormat::R8G8B8A8_UINT:
        return vk::Format::eR8G8B8A8Uint;
    case TextureFormat::R8G8B8A8_SINT:
        return vk::Format::eR8G8B8A8Sint;
    case TextureFormat::R8G8B8A8_SRGB:
        return vk::Format::eR8G8B8A8Srgb;

    case TextureFormat::B8G8R8A8_UNORM:
        return vk::Format::eB8G8R8A8Unorm;
    case TextureFormat::B8G8R8A8_SNORM:
        return vk::Format::eB8G8R8A8Snorm;
    case TextureFormat::B8G8R8A8_USCALED:
        return vk::Format::eB8G8R8A8Uscaled;
    case TextureFormat::B8G8R8A8_SSCALED:
        return vk::Format::eB8G8R8A8Sscaled;
    case TextureFormat::B8G8R8A8_UINT:
        return vk::Format::eB8G8R8A8Uint;
    case TextureFormat::B8G8R8A8_SINT:
        return vk::Format::eB8G8R8A8Sint;
    case TextureFormat::B8G8R8A8_SRGB:
        return vk::Format::eB8G8R8A8Srgb;

    case TextureFormat::R16_UNORM:
        return vk::Format::eR16Unorm;
    case TextureFormat::R16_SNORM:
        return vk::Format::eR16Snorm;
    case TextureFormat::R16_USCALED:
        return vk::Format::eR16Uscaled;
    case TextureFormat::R16_SSCALED:
        return vk::Format::eR16Sscaled;
    case TextureFormat::R16_UINT:
        return vk::Format::eR16Uint;
    case TextureFormat::R16_SINT:
        return vk::Format::eR16Sint;
    case TextureFormat::R16_SFLOAT:
        return vk::Format::eR16Sfloat;

    case TextureFormat::R16G16_UNORM:
        return vk::Format::eR16G16Unorm;
    case TextureFormat::R16G16_SNORM:
        return vk::Format::eR16G16Snorm;
    case TextureFormat::R16G16_USCALED:
        return vk::Format::eR16G16Uscaled;
    case TextureFormat::R16G16_SSCALED:
        return vk::Format::eR16G16Sscaled;
    case TextureFormat::R16G16_UINT:
        return vk::Format::eR16G16Uint;
    case TextureFormat::R16G16_SINT:
        return vk::Format::eR16G16Sint;
    case TextureFormat::R16G16_SFLOAT:
        return vk::Format::eR16G16Sfloat;

    case TextureFormat::R16G16B16_UNORM:
        return vk::Format::eR16G16B16Unorm;
    case TextureFormat::R16G16B16_SNORM:
        return vk::Format::eR16G16B16Snorm;
    case TextureFormat::R16G16B16_USCALED:
        return vk::Format::eR16G16B16Uscaled;
    case TextureFormat::R16G16B16_SSCALED:
        return vk::Format::eR16G16B16Sscaled;
    case TextureFormat::R16G16B16_UINT:
        return vk::Format::eR16G16B16Uint;
    case TextureFormat::R16G16B16_SINT:
        return vk::Format::eR16G16B16Sint;
    case TextureFormat::R16G16B16_SFLOAT:
        return vk::Format::eR16G16B16Sfloat;

    case TextureFormat::R16G16B16A16_UNORM:
        return vk::Format::eR16G16B16A16Unorm;
    case TextureFormat::R16G16B16A16_SNORM:
        return vk::Format::eR16G16B16A16Snorm;
    case TextureFormat::R16G16B16A16_USCALED:
        return vk::Format::eR16G16B16A16Uscaled;
    case TextureFormat::R16G16B16A16_SSCALED:
        return vk::Format::eR16G16B16A16Sscaled;
    case TextureFormat::R16G16B16A16_UINT:
        return vk::Format::eR16G16B16A16Uint;
    case TextureFormat::R16G16B16A16_SINT:
        return vk::Format::eR16G16B16A16Sint;
    case TextureFormat::R16G16B16A16_SFLOAT:
        return vk::Format::eR16G16B16A16Sfloat;

    case TextureFormat::R32_UINT:
        return vk::Format::eR32Uint;
    case TextureFormat::R32_SINT:
        return vk::Format::eR32Sint;
    case TextureFormat::R32_SFLOAT:
        return vk::Format::eR32Sfloat;

    case TextureFormat::R32G32_UINT:
        return vk::Format::eR32G32Uint;
    case TextureFormat::R32G32_SINT:
        return vk::Format::eR32G32Sint;
    case TextureFormat::R32G32_SFLOAT:
        return vk::Format::eR32G32Sfloat;

    case TextureFormat::R32G32B32_UINT:
        return vk::Format::eR32G32B32Uint;
    case TextureFormat::R32G32B32_SINT:
        return vk::Format::eR32G32B32Sint;
    case TextureFormat::R32G32B32_SFLOAT:
        return vk::Format::eR32G32B32Sfloat;

    case TextureFormat::R32G32B32A32_UINT:
        return vk::Format::eR32G32B32A32Uint;
    case TextureFormat::R32G32B32A32_SINT:
        return vk::Format::eR32G32B32A32Sint;
    case TextureFormat::R32G32B32A32_SFLOAT:
        return vk::Format::eR32G32B32A32Sfloat;

    case TextureFormat::R64_UINT:
        return vk::Format::eR64Uint;
    case TextureFormat::R64_SINT:
        return vk::Format::eR64Sint;
    case TextureFormat::R64_SFLOAT:
        return vk::Format::eR64Sfloat;

    case TextureFormat::R64G64_UINT:
        return vk::Format::eR64G64Uint;
    case TextureFormat::R64G64_SINT:
        return vk::Format::eR64G64Sint;
    case TextureFormat::R64G64_SFLOAT:
        return vk::Format::eR64G64Sfloat;

    case TextureFormat::R64G64B64_UINT:
        return vk::Format::eR64G64B64Uint;
    case TextureFormat::R64G64B64_SINT:
        return vk::Format::eR64G64B64Sint;
    case TextureFormat::R64G64B64_SFLOAT:
        return vk::Format::eR64G64B64Sfloat;

    case TextureFormat::R64G64B64A64_UINT:
        return vk::Format::eR64G64B64A64Uint;
    case TextureFormat::R64G64B64A64_SINT:
        return vk::Format::eR64G64B64A64Sint;
    case TextureFormat::R64G64B64A64_SFLOAT:
        return vk::Format::eR64G64B64A64Sfloat;

    case TextureFormat::D16_UNORM:
        return vk::Format::eD16Unorm;
    case TextureFormat::D32_SFLOAT:
        return vk::Format::eD32Sfloat;
    case TextureFormat::S8_UINT:
        return vk::Format::eS8Uint;
    case TextureFormat::D16_UNORM_S8_UINT:
        return vk::Format::eD16UnormS8Uint;
    case TextureFormat::D24_UNORM_S8_UINT:
        return vk::Format::eD24UnormS8Uint;
    case TextureFormat::D32_SFLOAT_S8_UINT:
        return vk::Format::eD32SfloatS8Uint;
    case TextureFormat::UNKNOWN:
    default:
        return vk::Format::eUndefined;
    }
}

inline GPUType getGPUType(const vk::PhysicalDeviceType type)
{
    switch (type)
    {
    case vk::PhysicalDeviceType::eIntegratedGpu:
        return GPUType::INTEGRATED;
    case vk::PhysicalDeviceType::eDiscreteGpu:
        return GPUType::DISCRETE;
    default:
        return GPUType::UNKNOWN;
    }
}

inline SampleCountFlags getSampleCountFlags(const vk::SampleCountFlags flags)
{
    return static_cast<SampleCountFlags>(static_cast<uint32_t>(flags));
}

inline QueueFamilyFlags getQueueFamilyFlags(const vk::QueueFlags flags,
                                            const bool presentation_support)
{
    auto res = QueueFamilyFlags::NONE;
    if ((flags & vk::QueueFlagBits::eGraphics) == vk::QueueFlagBits::eGraphics)
    {
        res = res | QueueFamilyFlags::GRAPHICS;
    }
    if ((flags & vk::QueueFlagBits::eCompute) == vk::QueueFlagBits::eCompute)
    {
        res = res | QueueFamilyFlags::COMPUTE;
    }
    if ((flags & vk::QueueFlagBits::eTransfer) == vk::QueueFlagBits::eTransfer)
    {
        res = res | QueueFamilyFlags::TRANSFER;
    }
    if (presentation_support)
    {
        res = res | QueueFamilyFlags::PRESENTATION;
    }
    return res;
}

inline std::vector<QueueFamily> getQueueFamilies(const vk::PhysicalDevice device, const ::vk::SurfaceKHR surface)
{
    const std::vector<vk::QueueFamilyProperties> props =
        device.getQueueFamilyProperties();
    std::vector<QueueFamily> queue_families;
    for (uint32_t i = 0; i < props.size(); i++)
    {
        const bool presentation_supported =
            surface != nullptr &&
            device.getSurfaceSupportKHR(i, surface);
        queue_families.emplace_back(QueueFamily{
            i, props[i].queueCount,
            getQueueFamilyFlags(props[i].queueFlags, presentation_supported)
        });
    }
    return queue_families;
}

inline GPUFeatures getGPUFeatures(const vk::PhysicalDevice device)
{
    vk::PhysicalDeviceRayTracingPipelineFeaturesKHR raytracing_features = {};
    vk::PhysicalDeviceAccelerationStructureFeaturesKHR accel_struct_features = {};
    accel_struct_features.pNext = &raytracing_features;
    vk::PhysicalDeviceDescriptorIndexingFeatures bindless_features = {};
    bindless_features.pNext = &accel_struct_features;
    vk::PhysicalDeviceShaderAtomicInt64Features atomic64_features = {};
    atomic64_features.pNext = &bindless_features;
    vk::PhysicalDeviceShaderFloat16Int8Features f16_features = {};
    f16_features.pNext = &atomic64_features;
    vk::PhysicalDeviceFeatures2 features = {};
    features.pNext = &f16_features;
    device.getFeatures2(&features);

    GPUFeatures gpu_features = {};
    gpu_features.robust_buffer_access = features.features.robustBufferAccess;
    gpu_features.full_draw_index_uint32 = features.features.fullDrawIndexUint32;
    gpu_features.image_cube_array = features.features.imageCubeArray;
    gpu_features.independent_blend = features.features.independentBlend;
    gpu_features.geometry_shader = features.features.geometryShader;
    gpu_features.tessellation_shader = features.features.tessellationShader;
    gpu_features.sample_rate_shading = features.features.sampleRateShading;
    gpu_features.dual_src_blend = features.features.dualSrcBlend;
    gpu_features.logic_op = features.features.logicOp;
    gpu_features.multi_draw_indirect = features.features.multiDrawIndirect;
    gpu_features.draw_indirect_first_instance =
        features.features.drawIndirectFirstInstance;
    gpu_features.depth_clamp = features.features.depthClamp;
    gpu_features.depth_bias_clamp = features.features.depthBiasClamp;
    gpu_features.fill_mode_non_solid = features.features.fillModeNonSolid;
    gpu_features.depth_bounds = features.features.depthBounds;
    gpu_features.wide_lines = features.features.wideLines;
    gpu_features.large_points = features.features.largePoints;
    gpu_features.alpha_to_one = features.features.alphaToOne;
    gpu_features.multi_viewport = features.features.multiViewport;
    gpu_features.sampler_anisotropy = features.features.samplerAnisotropy;
    gpu_features.texture_compression_ETC2 =
        features.features.textureCompressionETC2;
    gpu_features.texture_compression_ASTCLDR =
        features.features.textureCompressionASTC_LDR;
    gpu_features.texture_compression_BC = features.features.textureCompressionBC;
    gpu_features.occlusion_query_precise =
        features.features.occlusionQueryPrecise;
    gpu_features.pipeline_statistics_query =
        features.features.pipelineStatisticsQuery;
    gpu_features.vertex_pipeline_stores_and_atomics =
        features.features.vertexPipelineStoresAndAtomics;
    gpu_features.fragment_stores_and_atomics =
        features.features.fragmentStoresAndAtomics;
    gpu_features.shader_tessellation_and_geometry_point_size =
        features.features.shaderTessellationAndGeometryPointSize;
    gpu_features.shader_image_gather_extended =
        features.features.shaderImageGatherExtended;
    gpu_features.shader_storage_image_extended_formats =
        features.features.shaderStorageImageExtendedFormats;
    gpu_features.shader_storage_image_multisample =
        features.features.shaderStorageImageMultisample;
    gpu_features.shader_storage_image_read_without_format =
        features.features.shaderStorageImageReadWithoutFormat;
    gpu_features.shader_storage_image_write_without_format =
        features.features.shaderStorageImageWriteWithoutFormat;
    gpu_features.shader_uniform_buffer_array_dynamic_indexing =
        features.features.shaderUniformBufferArrayDynamicIndexing;
    gpu_features.shader_sampled_image_array_dynamic_indexing =
        features.features.shaderSampledImageArrayDynamicIndexing;
    gpu_features.shader_storage_buffer_array_dynamic_indexing =
        features.features.shaderStorageBufferArrayDynamicIndexing;
    gpu_features.shader_storage_image_array_dynamic_indexing =
        features.features.shaderStorageImageArrayDynamicIndexing;
    gpu_features.shader_clip_distance = features.features.shaderClipDistance;
    gpu_features.shader_cull_distance = features.features.shaderCullDistance;
    gpu_features.shader_float64 = features.features.shaderFloat64;
    gpu_features.shader_int64 = features.features.shaderInt64;
    gpu_features.shader_int16 = features.features.shaderInt16;
    gpu_features.shader_resource_residency =
        features.features.shaderResourceResidency;
    gpu_features.shader_resource_min_lod = features.features.shaderResourceMinLod;
    gpu_features.sparse_binding = features.features.sparseBinding;
    gpu_features.sparse_residency_buffer =
        features.features.sparseResidencyBuffer;
    gpu_features.sparse_residency_image2D =
        features.features.sparseResidencyImage2D;
    gpu_features.sparse_residency_image3D =
        features.features.sparseResidencyImage3D;
    gpu_features.sparse_residency2_samples =
        features.features.sparseResidency2Samples;
    gpu_features.sparse_residency4_samples =
        features.features.sparseResidency4Samples;
    gpu_features.sparse_residency8_samples =
        features.features.sparseResidency8Samples;
    gpu_features.sparse_residency16_samples =
        features.features.sparseResidency16Samples;
    gpu_features.sparse_residency_aliased =
        features.features.sparseResidencyAliased;
    gpu_features.variable_multisample_rate =
        features.features.variableMultisampleRate;
    gpu_features.inherited_queries = features.features.inheritedQueries;
    // 16-bit float 8-bit int support
    gpu_features.shader_float16 = f16_features.shaderFloat16;
    gpu_features.shader_int8 = f16_features.shaderInt8;
    // int 64-bit atomic features
    gpu_features.shader_buffer_int64_atomics =
        atomic64_features.shaderBufferInt64Atomics;
    gpu_features.shader_shared_int64_atomics =
        atomic64_features.shaderSharedInt64Atomics;
    // Bindless support features
    gpu_features.shader_input_attachment_array_dynamic_indexing =
        bindless_features.shaderInputAttachmentArrayDynamicIndexing;
    gpu_features.shader_uniform_texel_buffer_array_dynamic_indexing =
        bindless_features.shaderUniformTexelBufferArrayDynamicIndexing;
    gpu_features.shader_storage_texel_buffer_array_dynamic_indexing =
        bindless_features.shaderStorageTexelBufferArrayDynamicIndexing;
    gpu_features.shader_uniform_buffer_array_non_uniform_indexing =
        bindless_features.shaderUniformBufferArrayNonUniformIndexing;
    gpu_features.shader_sampled_image_array_non_uniform_indexing =
        bindless_features.shaderSampledImageArrayNonUniformIndexing;
    gpu_features.shader_storage_buffer_array_non_uniform_indexing =
        bindless_features.shaderStorageBufferArrayNonUniformIndexing;
    gpu_features.shader_storage_image_array_non_uniform_indexing =
        bindless_features.shaderStorageImageArrayNonUniformIndexing;
    gpu_features.shader_input_attachment_array_non_uniform_indexing =
        bindless_features.shaderInputAttachmentArrayNonUniformIndexing;
    gpu_features.shader_uniform_texel_buffer_array_non_uniform_indexing =
        bindless_features.shaderUniformTexelBufferArrayNonUniformIndexing;
    gpu_features.shader_storage_texel_buffer_array_non_uniform_indexing =
        bindless_features.shaderStorageTexelBufferArrayNonUniformIndexing;
    gpu_features.descriptor_binding_uniform_buffer_update_after_bind =
        bindless_features.descriptorBindingUniformBufferUpdateAfterBind;
    gpu_features.descriptor_binding_sampled_image_update_after_bind =
        bindless_features.descriptorBindingSampledImageUpdateAfterBind;
    gpu_features.descriptor_binding_storage_image_update_after_bind =
        bindless_features.descriptorBindingStorageImageUpdateAfterBind;
    gpu_features.descriptor_binding_storage_buffer_update_after_bind =
        bindless_features.descriptorBindingStorageBufferUpdateAfterBind;
    gpu_features.descriptor_binding_uniform_texel_buffer_update_after_bind =
        bindless_features.descriptorBindingUniformTexelBufferUpdateAfterBind;
    gpu_features.descriptor_binding_storage_texel_buffer_update_after_bind =
        bindless_features.descriptorBindingStorageTexelBufferUpdateAfterBind;
    gpu_features.descriptor_binding_update_unused_while_pending =
        bindless_features.descriptorBindingUpdateUnusedWhilePending;
    gpu_features.descriptor_binding_partially_bound =
        bindless_features.descriptorBindingPartiallyBound;
    gpu_features.descriptor_binding_variable_descriptor_count =
        bindless_features.descriptorBindingVariableDescriptorCount;
    gpu_features.runtime_descriptor_array =
        bindless_features.runtimeDescriptorArray;
    // Acceleration Structure features
    gpu_features.acceleration_structure =
        accel_struct_features.accelerationStructure;
    gpu_features.acceleration_structure_capture_replay =
        accel_struct_features.accelerationStructureCaptureReplay;
    gpu_features.acceleration_structure_indirect_build =
        accel_struct_features.accelerationStructureIndirectBuild;
    gpu_features.acceleration_structure_host_commands =
        accel_struct_features.accelerationStructureHostCommands;
    gpu_features.descriptor_binding_acceleration_structure_update_after_bind =
        accel_struct_features
        .descriptorBindingAccelerationStructureUpdateAfterBind;
    // Raytracing features
    gpu_features.ray_tracing_pipeline = raytracing_features.rayTracingPipeline;
    gpu_features.ray_tracing_pipeline_shader_group_handle_capture_replay =
        raytracing_features.rayTracingPipelineShaderGroupHandleCaptureReplay;
    gpu_features.ray_tracing_pipeline_shader_group_handle_capture_replay_mixed =
        raytracing_features.rayTracingPipelineShaderGroupHandleCaptureReplayMixed;
    gpu_features.ray_tracing_pipeline_trace_rays_indirect =
        raytracing_features.rayTracingPipelineTraceRaysIndirect;
    gpu_features.ray_traversal_primitive_culling =
        raytracing_features.rayTraversalPrimitiveCulling;

    return gpu_features;
}

inline GPULimits getGPULimits(const vk::PhysicalDevice device)
{
    vk::PhysicalDeviceRayTracingPipelinePropertiesKHR raytracing_props = {};
    vk::PhysicalDeviceAccelerationStructurePropertiesKHR accel_struct_props = {};
    accel_struct_props.pNext = &raytracing_props;
    vk::PhysicalDeviceDescriptorIndexingProperties bindless_props = {};
    bindless_props.pNext = &accel_struct_props;
    vk::PhysicalDeviceProperties2 props = {};
    props.pNext = &bindless_props;
    device.getProperties2(&props);

    GPULimits limits = {};
    limits.max_image_dimension_1D = props.properties.limits.maxImageDimension1D;
    limits.max_image_dimension_2D = props.properties.limits.maxImageDimension2D;
    limits.max_image_dimension_3D = props.properties.limits.maxImageDimension3D;
    limits.max_image_dimension_cube =
        props.properties.limits.maxImageDimensionCube;
    limits.max_image_array_layers = props.properties.limits.maxImageArrayLayers;
    limits.max_texel_buffer_elements =
        props.properties.limits.maxTexelBufferElements;
    limits.max_uniform_buffer_range =
        props.properties.limits.maxUniformBufferRange;
    limits.max_storage_buffer_range =
        props.properties.limits.maxStorageBufferRange;
    limits.max_push_constants_size = props.properties.limits.maxPushConstantsSize;
    limits.max_memory_allocation_count =
        props.properties.limits.maxMemoryAllocationCount;
    limits.max_sampler_allocation_count =
        props.properties.limits.maxSamplerAllocationCount;
    limits.buffer_image_granularity =
        props.properties.limits.bufferImageGranularity;
    limits.sparse_address_space_size =
        props.properties.limits.sparseAddressSpaceSize;
    limits.max_bound_descriptor_sets =
        props.properties.limits.maxBoundDescriptorSets;
    limits.max_per_stage_descriptor_samplers =
        props.properties.limits.maxPerStageDescriptorSamplers;
    limits.max_per_stage_descriptor_uniform_buffers =
        props.properties.limits.maxPerStageDescriptorUniformBuffers;
    limits.max_per_stage_descriptor_storage_buffers =
        props.properties.limits.maxPerStageDescriptorStorageBuffers;
    limits.max_per_stage_descriptor_sampled_images =
        props.properties.limits.maxPerStageDescriptorSampledImages;
    limits.max_per_stage_descriptor_storage_images =
        props.properties.limits.maxPerStageDescriptorStorageImages;
    limits.max_per_stage_descriptor_input_attachments =
        props.properties.limits.maxPerStageDescriptorInputAttachments;
    limits.max_per_stage_resources = props.properties.limits.maxPerStageResources;
    limits.max_descriptor_set_samplers =
        props.properties.limits.maxDescriptorSetSamplers;
    limits.max_descriptor_set_uniform_buffers =
        props.properties.limits.maxDescriptorSetUniformBuffers;
    limits.max_descriptor_set_uniform_buffers_dynamic =
        props.properties.limits.maxDescriptorSetUniformBuffersDynamic;
    limits.max_descriptor_set_storage_buffers =
        props.properties.limits.maxDescriptorSetStorageBuffers;
    limits.max_descriptor_set_storage_buffers_dynamic =
        props.properties.limits.maxDescriptorSetStorageBuffersDynamic;
    limits.max_descriptor_set_sampled_images =
        props.properties.limits.maxDescriptorSetSampledImages;
    limits.max_descriptor_set_storage_images =
        props.properties.limits.maxDescriptorSetStorageImages;
    limits.max_descriptor_set_input_attachments =
        props.properties.limits.maxDescriptorSetInputAttachments;
    limits.max_vertex_input_attributes =
        props.properties.limits.maxVertexInputAttributes;
    limits.max_vertex_input_bindings =
        props.properties.limits.maxVertexInputBindings;
    limits.max_vertex_input_attribute_offset =
        props.properties.limits.maxVertexInputAttributeOffset;
    limits.max_vertex_input_binding_stride =
        props.properties.limits.maxVertexInputBindingStride;
    limits.max_vertex_output_components =
        props.properties.limits.maxVertexOutputComponents;
    limits.max_tessellation_generation_level =
        props.properties.limits.maxTessellationGenerationLevel;
    limits.max_tessellation_patch_size =
        props.properties.limits.maxTessellationPatchSize;
    limits.max_tessellation_control_per_vertex_input_components =
        props.properties.limits.maxTessellationControlPerVertexInputComponents;
    limits.max_tessellation_control_per_vertex_output_components =
        props.properties.limits.maxTessellationControlPerVertexOutputComponents;
    limits.max_tessellation_control_per_patch_output_components =
        props.properties.limits.maxTessellationControlPerPatchOutputComponents;
    limits.max_tessellation_control_total_output_components =
        props.properties.limits.maxTessellationControlTotalOutputComponents;
    limits.max_tessellation_evaluation_input_components =
        props.properties.limits.maxTessellationEvaluationInputComponents;
    limits.max_tessellation_evaluation_output_components =
        props.properties.limits.maxTessellationEvaluationOutputComponents;
    limits.max_geometry_shader_invocations =
        props.properties.limits.maxGeometryShaderInvocations;
    limits.max_geometry_input_components =
        props.properties.limits.maxGeometryInputComponents;
    limits.max_geometry_output_components =
        props.properties.limits.maxGeometryOutputComponents;
    limits.max_geometry_output_vertices =
        props.properties.limits.maxGeometryOutputVertices;
    limits.max_geometry_total_output_components =
        props.properties.limits.maxGeometryTotalOutputComponents;
    limits.max_fragment_input_components =
        props.properties.limits.maxFragmentInputComponents;
    limits.max_fragment_output_attachments =
        props.properties.limits.maxFragmentOutputAttachments;
    limits.max_fragment_dual_src_attachments =
        props.properties.limits.maxFragmentDualSrcAttachments;
    limits.max_fragment_combined_output_resources =
        props.properties.limits.maxFragmentCombinedOutputResources;
    limits.max_compute_shared_memory_size =
        props.properties.limits.maxComputeSharedMemorySize;
    limits.max_compute_work_group_count =
        props.properties.limits.maxComputeWorkGroupCount;
    limits.max_compute_work_group_invocations =
        props.properties.limits.maxComputeWorkGroupInvocations;
    limits.max_compute_work_group_size =
        props.properties.limits.maxComputeWorkGroupSize;
    limits.sub_pixel_precision_bits =
        props.properties.limits.subPixelPrecisionBits;
    limits.sub_texel_precision_bits =
        props.properties.limits.subTexelPrecisionBits;
    limits.mipmap_precision_bits = props.properties.limits.mipmapPrecisionBits;
    limits.max_draw_indexed_index_value =
        props.properties.limits.maxDrawIndexedIndexValue;
    limits.max_draw_indirect_count = props.properties.limits.maxDrawIndirectCount;
    limits.max_sampler_lod_bias = props.properties.limits.maxSamplerLodBias;
    limits.max_sampler_anisotropy = props.properties.limits.maxSamplerAnisotropy;
    limits.max_viewports = props.properties.limits.maxViewports;
    limits.max_viewport_dimensions =
        props.properties.limits.maxViewportDimensions;
    limits.viewport_bounds_range = props.properties.limits.viewportBoundsRange;
    limits.viewport_sub_pixel_bits = props.properties.limits.viewportSubPixelBits;
    limits.min_memory_map_alignment =
        props.properties.limits.minMemoryMapAlignment;
    limits.min_texel_buffer_offset_alignment =
        props.properties.limits.minTexelBufferOffsetAlignment;
    limits.min_uniform_buffer_offset_alignment =
        props.properties.limits.minUniformBufferOffsetAlignment;
    limits.min_storage_buffer_offset_alignment =
        props.properties.limits.minStorageBufferOffsetAlignment;
    limits.min_texel_offset = props.properties.limits.minTexelOffset;
    limits.max_texel_offset = props.properties.limits.maxTexelOffset;
    limits.min_texel_gather_offset = props.properties.limits.minTexelGatherOffset;
    limits.max_texel_gather_offset = props.properties.limits.maxTexelGatherOffset;
    limits.min_interpolation_offset =
        props.properties.limits.minInterpolationOffset;
    limits.max_interpolation_offset =
        props.properties.limits.maxInterpolationOffset;
    limits.sub_pixel_interpolation_offset_bits =
        props.properties.limits.subPixelInterpolationOffsetBits;
    limits.max_framebuffer_width = props.properties.limits.maxFramebufferWidth;
    limits.max_framebuffer_height = props.properties.limits.maxFramebufferHeight;
    limits.max_framebuffer_layers = props.properties.limits.maxFramebufferLayers;
    limits.framebuffer_color_sample_counts =
        getSampleCountFlags(props.properties.limits.framebufferColorSampleCounts);
    limits.framebuffer_depth_sample_counts =
        getSampleCountFlags(props.properties.limits.framebufferDepthSampleCounts);
    limits.framebuffer_stencil_sample_counts = getSampleCountFlags(
        props.properties.limits.framebufferStencilSampleCounts);
    limits.framebuffer_no_attachments_sample_counts = getSampleCountFlags(
        props.properties.limits.framebufferNoAttachmentsSampleCounts);
    limits.max_color_attachments = props.properties.limits.maxColorAttachments;
    limits.sampled_image_color_sample_counts = getSampleCountFlags(
        props.properties.limits.sampledImageColorSampleCounts);
    limits.sampled_image_integer_sample_counts = getSampleCountFlags(
        props.properties.limits.sampledImageIntegerSampleCounts);
    limits.sampled_image_depth_sample_counts = getSampleCountFlags(
        props.properties.limits.sampledImageDepthSampleCounts);
    limits.sampled_image_stencil_sample_counts = getSampleCountFlags(
        props.properties.limits.sampledImageStencilSampleCounts);
    limits.storage_image_sample_counts =
        getSampleCountFlags(props.properties.limits.storageImageSampleCounts);
    limits.max_sample_mask_words = props.properties.limits.maxSampleMaskWords;
    limits.timestamp_compute_and_graphics =
        props.properties.limits.timestampComputeAndGraphics;
    limits.timestamp_period = props.properties.limits.timestampPeriod;
    limits.max_clip_distances = props.properties.limits.maxClipDistances;
    limits.max_cull_distances = props.properties.limits.maxCullDistances;
    limits.max_combined_clip_and_cull_distances =
        props.properties.limits.maxCombinedClipAndCullDistances;
    limits.discrete_queue_priorities =
        props.properties.limits.discreteQueuePriorities;
    limits.point_size_range = props.properties.limits.pointSizeRange;
    limits.line_width_range = props.properties.limits.lineWidthRange;
    limits.point_size_granularity = props.properties.limits.pointSizeGranularity;
    limits.line_width_granularity = props.properties.limits.lineWidthGranularity;
    limits.strict_lines = props.properties.limits.strictLines;
    limits.standard_sample_locations =
        props.properties.limits.standardSampleLocations;
    limits.optimal_buffer_copy_offset_alignment =
        props.properties.limits.optimalBufferCopyOffsetAlignment;
    limits.optimal_buffer_copy_row_pitch_alignment =
        props.properties.limits.optimalBufferCopyRowPitchAlignment;
    limits.non_coherent_atom_size = props.properties.limits.nonCoherentAtomSize;
    // bindless limits
    limits.max_update_after_bind_descriptors_in_all_pools =
        bindless_props.maxUpdateAfterBindDescriptorsInAllPools;
    limits.shader_uniform_buffer_array_non_uniform_indexing_native =
        bindless_props.shaderUniformBufferArrayNonUniformIndexingNative;
    limits.shader_sampled_image_array_non_uniform_indexing_native =
        bindless_props.shaderSampledImageArrayNonUniformIndexingNative;
    limits.shader_storage_buffer_array_non_uniform_indexing_native =
        bindless_props.shaderStorageBufferArrayNonUniformIndexingNative;
    limits.shader_storage_image_array_non_uniform_indexing_native =
        bindless_props.shaderStorageImageArrayNonUniformIndexingNative;
    limits.shader_input_attachment_array_non_uniform_indexing_native =
        bindless_props.shaderInputAttachmentArrayNonUniformIndexingNative;
    limits.robust_buffer_access_update_after_bind =
        bindless_props.robustBufferAccessUpdateAfterBind;
    limits.quad_divergent_implicit_lod = bindless_props.quadDivergentImplicitLod;
    limits.max_per_stage_descriptor_update_after_bind_samplers =
        bindless_props.maxPerStageDescriptorUpdateAfterBindSamplers;
    limits.max_per_stage_descriptor_update_after_bind_uniform_buffers =
        bindless_props.maxPerStageDescriptorUpdateAfterBindUniformBuffers;
    limits.max_per_stage_descriptor_update_after_bind_storage_buffers =
        bindless_props.maxPerStageDescriptorUpdateAfterBindStorageBuffers;
    limits.max_per_stage_descriptor_update_after_bind_sampled_images =
        bindless_props.maxPerStageDescriptorUpdateAfterBindSampledImages;
    limits.max_per_stage_descriptor_update_after_bind_storage_images =
        bindless_props.maxPerStageDescriptorUpdateAfterBindStorageImages;
    limits.max_per_stage_descriptor_update_after_bind_input_attachments =
        bindless_props.maxPerStageDescriptorUpdateAfterBindInputAttachments;
    limits.max_per_stage_update_after_bind_resources =
        bindless_props.maxPerStageUpdateAfterBindResources;
    limits.max_descriptor_set_update_after_bind_samplers =
        bindless_props.maxDescriptorSetUpdateAfterBindSamplers;
    limits.max_descriptor_set_update_after_bind_uniform_buffers =
        bindless_props.maxDescriptorSetUpdateAfterBindUniformBuffers;
    limits.max_descriptor_set_update_after_bind_uniform_buffers_dynamic =
        bindless_props.maxDescriptorSetUpdateAfterBindUniformBuffersDynamic;
    limits.max_descriptor_set_update_after_bind_storage_buffers =
        bindless_props.maxDescriptorSetUpdateAfterBindStorageBuffers;
    limits.max_descriptor_set_update_after_bind_storage_buffers_dynamic =
        bindless_props.maxDescriptorSetUpdateAfterBindStorageBuffersDynamic;
    limits.max_descriptor_set_update_after_bind_sampled_images =
        bindless_props.maxDescriptorSetUpdateAfterBindSampledImages;
    limits.max_descriptor_set_update_after_bind_storage_images =
        bindless_props.maxDescriptorSetUpdateAfterBindStorageImages;
    limits.max_descriptor_set_update_after_bind_input_attachments =
        bindless_props.maxDescriptorSetUpdateAfterBindInputAttachments;
    // Acceleration Structure limits
    limits.max_geometry_count = accel_struct_props.maxGeometryCount;
    limits.max_instance_count = accel_struct_props.maxInstanceCount;
    limits.max_primitive_count = accel_struct_props.maxPrimitiveCount;
    limits.max_per_stage_descriptor_acceleration_structures =
        accel_struct_props.maxPerStageDescriptorAccelerationStructures;
    limits.max_per_stage_descriptor_update_after_bind_acceleration_structures =
        accel_struct_props
        .maxPerStageDescriptorUpdateAfterBindAccelerationStructures;
    limits.max_descriptor_set_acceleration_structures =
        accel_struct_props.maxDescriptorSetAccelerationStructures;
    limits.max_descriptor_set_update_after_bind_acceleration_structures =
        accel_struct_props.maxDescriptorSetUpdateAfterBindAccelerationStructures;
    limits.min_acceleration_structure_scratch_offset_alignment =
        accel_struct_props.minAccelerationStructureScratchOffsetAlignment;
    // Raytracing limits
    limits.shader_group_handle_size = raytracing_props.shaderGroupHandleSize;
    limits.max_ray_recursion_depth = raytracing_props.maxRayRecursionDepth;
    limits.max_shader_group_stride = raytracing_props.maxShaderGroupStride;
    limits.shader_group_base_alignment =
        raytracing_props.shaderGroupBaseAlignment;
    limits.shader_group_handle_capture_replay_size =
        raytracing_props.shaderGroupHandleCaptureReplaySize;
    limits.max_ray_dispatch_invocation_count =
        raytracing_props.maxRayDispatchInvocationCount;
    limits.shader_group_handle_alignment =
        raytracing_props.shaderGroupHandleAlignment;
    limits.max_ray_hit_attribute_size = raytracing_props.maxRayHitAttributeSize;

    return limits;
}


/**
 * Returns a score based on how many of the features are satisfied.
 * @param f The features of the device.
 * @param p The preferred features used to score.
 * @return A score.
 */
inline uint32_t getGPUFeatureScore(const GPUFeatures &f, const GPUFeatures &p)
{
    uint32_t score = 0;
    // Base Features
    if (p.robust_buffer_access && f.robust_buffer_access >= p.robust_buffer_access)
        score += 1;
    if (p.full_draw_index_uint32 && f.full_draw_index_uint32 >= p.full_draw_index_uint32)
        score += 1;
    if (p.image_cube_array && f.image_cube_array >= p.image_cube_array)
        score += 1;
    if (p.independent_blend && f.independent_blend >= p.independent_blend)
        score += 1;
    if (p.geometry_shader && f.geometry_shader >= p.geometry_shader)
        score += 1;
    if (p.tessellation_shader && f.tessellation_shader >= p.tessellation_shader)
        score += 1;
    if (p.sample_rate_shading && f.sample_rate_shading >= p.sample_rate_shading)
        score += 1;
    if (p.dual_src_blend && f.dual_src_blend >= p.dual_src_blend)
        score += 1;
    if (p.logic_op && f.logic_op >= p.logic_op)
        score += 1;
    if (p.multi_draw_indirect && f.multi_draw_indirect >= p.multi_draw_indirect)
        score += 1;
    if (p.draw_indirect_first_instance && f.draw_indirect_first_instance >= p.draw_indirect_first_instance)
        score +=
            1;
    if (p.depth_clamp && f.depth_clamp >= p.depth_clamp)
        score += 1;
    if (p.depth_bias_clamp && f.depth_bias_clamp >= p.depth_bias_clamp)
        score += 1;
    if (p.fill_mode_non_solid && f.fill_mode_non_solid >= p.fill_mode_non_solid)
        score += 1;
    if (p.depth_bounds && f.depth_bounds >= p.depth_bounds)
        score += 1;
    if (p.wide_lines && f.wide_lines >= p.wide_lines)
        score += 1;
    if (p.large_points && f.large_points >= p.large_points)
        score += 1;
    if (p.alpha_to_one && f.alpha_to_one >= p.alpha_to_one)
        score += 1;
    if (p.multi_viewport && f.multi_viewport >= p.multi_viewport)
        score += 1;
    if (p.sampler_anisotropy && f.sampler_anisotropy >= p.sampler_anisotropy)
        score += 1;
    if (p.texture_compression_ETC2 && f.texture_compression_ETC2 >= p.texture_compression_ETC2)
        score += 1;
    if (p.texture_compression_ASTCLDR && f.texture_compression_ASTCLDR >= p.texture_compression_ASTCLDR)
        score += 1;
    if (p.texture_compression_BC && f.texture_compression_BC >= p.texture_compression_BC)
        score += 1;
    if (p.occlusion_query_precise && f.occlusion_query_precise >= p.occlusion_query_precise)
        score += 1;
    if (p.pipeline_statistics_query && f.pipeline_statistics_query >= p.pipeline_statistics_query)
        score += 1;
    if (p.vertex_pipeline_stores_and_atomics && f.vertex_pipeline_stores_and_atomics >= p.
        vertex_pipeline_stores_and_atomics)
        score += 1;
    if (p.fragment_stores_and_atomics && f.fragment_stores_and_atomics >= p.fragment_stores_and_atomics)
        score += 1;
    if (p.shader_tessellation_and_geometry_point_size && f.shader_tessellation_and_geometry_point_size >= p.
        shader_tessellation_and_geometry_point_size)
        score += 1;
    if (p.shader_image_gather_extended && f.shader_image_gather_extended >= p.shader_image_gather_extended)
        score +=
            1;
    if (p.shader_storage_image_extended_formats && f.shader_storage_image_extended_formats >= p.
        shader_storage_image_extended_formats)
        score += 1;
    if (p.shader_storage_image_multisample && f.shader_storage_image_multisample >= p.
        shader_storage_image_multisample)
        score += 1;
    if (p.shader_storage_image_read_without_format && f.shader_storage_image_read_without_format >= p.
        shader_storage_image_read_without_format)
        score += 1;
    if (p.shader_storage_image_write_without_format && f.shader_storage_image_write_without_format >= p.
        shader_storage_image_write_without_format)
        score += 1;
    if (p.shader_uniform_buffer_array_dynamic_indexing && f.shader_uniform_buffer_array_dynamic_indexing >= p.
        shader_uniform_buffer_array_dynamic_indexing)
        score += 1;
    if (p.shader_sampled_image_array_dynamic_indexing && f.shader_sampled_image_array_dynamic_indexing >= p.
        shader_sampled_image_array_dynamic_indexing)
        score += 1;
    if (p.shader_storage_buffer_array_dynamic_indexing && f.shader_storage_buffer_array_dynamic_indexing >= p.
        shader_storage_buffer_array_dynamic_indexing)
        score += 1;
    if (p.shader_storage_image_array_dynamic_indexing && f.shader_storage_image_array_dynamic_indexing >= p.
        shader_storage_image_array_dynamic_indexing)
        score += 1;
    if (p.shader_clip_distance && f.shader_clip_distance >= p.shader_clip_distance)
        score += 1;
    if (p.shader_cull_distance && f.shader_cull_distance >= p.shader_cull_distance)
        score += 1;
    if (p.shader_float64 && f.shader_float64 >= p.shader_float64)
        score += 1;
    if (p.shader_int64 && f.shader_int64 >= p.shader_int64)
        score += 1;
    if (p.shader_int16 && f.shader_int16 >= p.shader_int16)
        score += 1;
    if (p.shader_resource_residency && f.shader_resource_residency >= p.shader_resource_residency)
        score += 1;
    if (p.shader_resource_min_lod && f.shader_resource_min_lod >= p.shader_resource_min_lod)
        score += 1;
    if (p.sparse_binding && f.sparse_binding >= p.sparse_binding)
        score += 1;
    if (p.sparse_residency_buffer && f.sparse_residency_buffer >= p.sparse_residency_buffer)
        score += 1;
    if (p.sparse_residency_image2D && f.sparse_residency_image2D >= p.sparse_residency_image2D)
        score += 1;
    if (p.sparse_residency_image3D && f.sparse_residency_image3D >= p.sparse_residency_image3D)
        score += 1;
    if (p.sparse_residency2_samples && f.sparse_residency2_samples >= p.sparse_residency2_samples)
        score += 1;
    if (p.sparse_residency4_samples && f.sparse_residency4_samples >= p.sparse_residency4_samples)
        score += 1;
    if (p.sparse_residency8_samples && f.sparse_residency8_samples >= p.sparse_residency8_samples)
        score += 1;
    if (p.sparse_residency16_samples && f.sparse_residency16_samples >= p.sparse_residency16_samples)
        score += 1;
    if (p.sparse_residency_aliased && f.sparse_residency_aliased >= p.sparse_residency_aliased)
        score += 1;
    if (p.variable_multisample_rate && f.variable_multisample_rate >= p.variable_multisample_rate)
        score += 1;
    if (p.inherited_queries && f.inherited_queries >= p.inherited_queries)
        score += 1;
    // 16-bit float 8-bit int features
    if (p.shader_float16 && f.shader_float16 >= p.shader_float16)
        score += 1;
    if (p.shader_int8 && f.shader_int8 >= p.shader_int8)
        score += 1;
    // int 64-bit atomic features
    if (p.shader_buffer_int64_atomics && f.shader_buffer_int64_atomics >= p.shader_buffer_int64_atomics)
        score += 1;
    if (p.shader_shared_int64_atomics && f.shader_shared_int64_atomics >= p.shader_shared_int64_atomics)
        score += 1;
    // Bindless support features
    if (p.shader_input_attachment_array_dynamic_indexing && f.shader_input_attachment_array_dynamic_indexing >= p.
        shader_input_attachment_array_dynamic_indexing)
        score += 1;
    if (p.shader_uniform_texel_buffer_array_dynamic_indexing && f.shader_uniform_texel_buffer_array_dynamic_indexing
        >= p.shader_uniform_texel_buffer_array_dynamic_indexing)
        score += 1;
    if (p.shader_storage_texel_buffer_array_dynamic_indexing && f.shader_storage_texel_buffer_array_dynamic_indexing
        >= p.shader_storage_texel_buffer_array_dynamic_indexing)
        score += 1;
    if (p.shader_uniform_buffer_array_non_uniform_indexing && f.shader_uniform_buffer_array_non_uniform_indexing >=
        p.shader_uniform_buffer_array_non_uniform_indexing)
        score += 1;
    if (p.shader_sampled_image_array_non_uniform_indexing && f.shader_sampled_image_array_non_uniform_indexing >= p.
        shader_sampled_image_array_non_uniform_indexing)
        score += 1;
    if (p.shader_storage_buffer_array_non_uniform_indexing && f.shader_storage_buffer_array_non_uniform_indexing >=
        p.shader_storage_buffer_array_non_uniform_indexing)
        score += 1;
    if (p.shader_storage_image_array_non_uniform_indexing && f.shader_storage_image_array_non_uniform_indexing >= p.
        shader_storage_image_array_non_uniform_indexing)
        score += 1;
    if (p.shader_input_attachment_array_non_uniform_indexing && f.shader_input_attachment_array_non_uniform_indexing
        >= p.shader_input_attachment_array_non_uniform_indexing)
        score += 1;
    if (p.shader_uniform_texel_buffer_array_non_uniform_indexing && f.
        shader_uniform_texel_buffer_array_non_uniform_indexing >= p.
        shader_uniform_texel_buffer_array_non_uniform_indexing)
        score += 1;
    if (p.shader_storage_texel_buffer_array_non_uniform_indexing && f.
        shader_storage_texel_buffer_array_non_uniform_indexing >= p.
        shader_storage_texel_buffer_array_non_uniform_indexing)
        score += 1;
    if (p.descriptor_binding_uniform_buffer_update_after_bind && f.
        descriptor_binding_uniform_buffer_update_after_bind >= p.
        descriptor_binding_uniform_buffer_update_after_bind)
        score += 1;
    if (p.descriptor_binding_sampled_image_update_after_bind && f.descriptor_binding_sampled_image_update_after_bind
        >= p.descriptor_binding_sampled_image_update_after_bind)
        score += 1;
    if (p.descriptor_binding_storage_image_update_after_bind && f.descriptor_binding_storage_image_update_after_bind
        >= p.descriptor_binding_storage_image_update_after_bind)
        score += 1;
    if (p.descriptor_binding_storage_buffer_update_after_bind && f.
        descriptor_binding_storage_buffer_update_after_bind >= p.
        descriptor_binding_storage_buffer_update_after_bind)
        score += 1;
    if (p.descriptor_binding_uniform_texel_buffer_update_after_bind && f.
        descriptor_binding_uniform_texel_buffer_update_after_bind >= p.
        descriptor_binding_uniform_texel_buffer_update_after_bind)
        score += 1;
    if (p.descriptor_binding_storage_texel_buffer_update_after_bind && f.
        descriptor_binding_storage_texel_buffer_update_after_bind >= p.
        descriptor_binding_storage_texel_buffer_update_after_bind)
        score += 1;
    if (p.descriptor_binding_update_unused_while_pending && f.descriptor_binding_update_unused_while_pending >= p.
        descriptor_binding_update_unused_while_pending)
        score += 1;
    if (p.descriptor_binding_partially_bound && f.descriptor_binding_partially_bound >= p.
        descriptor_binding_partially_bound)
        score += 1;
    if (p.descriptor_binding_variable_descriptor_count && f.descriptor_binding_variable_descriptor_count >= p.
        descriptor_binding_variable_descriptor_count)
        score += 1;
    if (p.runtime_descriptor_array && f.runtime_descriptor_array >= p.runtime_descriptor_array)
        score += 1;
    // Acceleration Structure features
    if (p.acceleration_structure && f.acceleration_structure >= p.acceleration_structure)
        score += 1;
    if (p.acceleration_structure_capture_replay && f.acceleration_structure_capture_replay >= p.
        acceleration_structure_capture_replay)
        score += 1;
    if (p.acceleration_structure_indirect_build && f.acceleration_structure_indirect_build >= p.
        acceleration_structure_indirect_build)
        score += 1;
    if (p.acceleration_structure_host_commands && f.acceleration_structure_host_commands >= p.
        acceleration_structure_host_commands)
        score += 1;
    if (p.descriptor_binding_acceleration_structure_update_after_bind && f.
        descriptor_binding_acceleration_structure_update_after_bind >= p.
        descriptor_binding_acceleration_structure_update_after_bind)
        score += 1;
    // Raytracing features
    if (p.ray_tracing_pipeline && f.ray_tracing_pipeline >= p.ray_tracing_pipeline)
        score += 1;
    if (p.ray_tracing_pipeline_shader_group_handle_capture_replay && f.
        ray_tracing_pipeline_shader_group_handle_capture_replay >= p.
        ray_tracing_pipeline_shader_group_handle_capture_replay)
        score += 1;
    if (p.ray_tracing_pipeline_shader_group_handle_capture_replay_mixed && f.
        ray_tracing_pipeline_shader_group_handle_capture_replay_mixed >= p.
        ray_tracing_pipeline_shader_group_handle_capture_replay_mixed)
        score += 1;
    if (p.ray_tracing_pipeline_trace_rays_indirect && f.ray_tracing_pipeline_trace_rays_indirect >= p.
        ray_tracing_pipeline_trace_rays_indirect)
        score += 1;
    if (p.ray_traversal_primitive_culling && f.ray_traversal_primitive_culling >= p.ray_traversal_primitive_culling)
        score += 1;
    return score;
}

/**
 * Returns a score based on how many of the limits are satisfied.
 * @param l The limits of the device.
 * @param p The preferred limits used to score.
 * @return A score.
 */
inline uint32_t getGPULimitScore(const GPULimits &l, const GPULimits &p)
{
    uint32_t score = 0;
    if (p.max_image_dimension_1D > 0 && l.max_image_dimension_1D >= p.max_image_dimension_1D)
        score += 1;
    if (p.max_image_dimension_2D > 0 && l.max_image_dimension_2D >= p.max_image_dimension_2D)
        score += 1;
    if (p.max_image_dimension_3D > 0 && l.max_image_dimension_3D >= p.max_image_dimension_3D)
        score += 1;
    if (p.max_image_dimension_cube > 0 && l.max_image_dimension_cube >= p.max_image_dimension_cube)
        score += 1;
    if (p.max_image_array_layers > 0 && l.max_image_array_layers >= p.max_image_array_layers)
        score += 1;
    if (p.max_texel_buffer_elements > 0 && l.max_texel_buffer_elements >= p.max_texel_buffer_elements)
        score += 1;
    if (p.max_uniform_buffer_range > 0 && l.max_uniform_buffer_range >= p.max_uniform_buffer_range)
        score += 1;
    if (p.max_storage_buffer_range > 0 && l.max_storage_buffer_range >= p.max_storage_buffer_range)
        score += 1;
    if (p.max_push_constants_size > 0 && l.max_push_constants_size >= p.max_push_constants_size)
        score += 1;
    if (p.max_memory_allocation_count > 0 && l.max_memory_allocation_count >= p.max_memory_allocation_count)
        score
            += 1;
    if (p.max_sampler_allocation_count > 0 && l.max_sampler_allocation_count >= p.max_sampler_allocation_count)
        score += 1;
    if (p.buffer_image_granularity > 0 && l.buffer_image_granularity >= p.buffer_image_granularity)
        score += 1;
    if (p.sparse_address_space_size > 0 && l.sparse_address_space_size >= p.sparse_address_space_size)
        score += 1;
    if (p.max_bound_descriptor_sets > 0 && l.max_bound_descriptor_sets >= p.max_bound_descriptor_sets)
        score += 1;
    if (p.max_per_stage_descriptor_samplers > 0 && l.max_per_stage_descriptor_samplers >= p.
        max_per_stage_descriptor_samplers)
        score += 1;
    if (p.max_per_stage_descriptor_uniform_buffers > 0 && l.max_per_stage_descriptor_uniform_buffers >= p.
        max_per_stage_descriptor_uniform_buffers)
        score += 1;
    if (p.max_per_stage_descriptor_storage_buffers > 0 && l.max_per_stage_descriptor_storage_buffers >= p.
        max_per_stage_descriptor_storage_buffers)
        score += 1;
    if (p.max_per_stage_descriptor_sampled_images > 0 && l.max_per_stage_descriptor_sampled_images >= p.
        max_per_stage_descriptor_sampled_images)
        score += 1;
    if (p.max_per_stage_descriptor_storage_images > 0 && l.max_per_stage_descriptor_storage_images >= p.
        max_per_stage_descriptor_storage_images)
        score += 1;
    if (p.max_per_stage_descriptor_input_attachments > 0 && l.max_per_stage_descriptor_input_attachments >= p.
        max_per_stage_descriptor_input_attachments)
        score += 1;
    if (p.max_per_stage_resources > 0 && l.max_per_stage_resources >= p.max_per_stage_resources)
        score += 1;
    if (p.max_descriptor_set_samplers > 0 && l.max_descriptor_set_samplers >= p.max_descriptor_set_samplers)
        score
            += 1;
    if (p.max_descriptor_set_uniform_buffers > 0 && l.max_descriptor_set_uniform_buffers >= p.
        max_descriptor_set_uniform_buffers)
        score += 1;
    if (p.max_descriptor_set_uniform_buffers_dynamic > 0 && l.max_descriptor_set_uniform_buffers_dynamic >= p.
        max_descriptor_set_uniform_buffers_dynamic)
        score += 1;
    if (p.max_descriptor_set_storage_buffers > 0 && l.max_descriptor_set_storage_buffers >= p.
        max_descriptor_set_storage_buffers)
        score += 1;
    if (p.max_descriptor_set_storage_buffers_dynamic > 0 && l.max_descriptor_set_storage_buffers_dynamic >= p.
        max_descriptor_set_storage_buffers_dynamic)
        score += 1;
    if (p.max_descriptor_set_sampled_images > 0 && l.max_descriptor_set_sampled_images >= p.
        max_descriptor_set_sampled_images)
        score += 1;
    if (p.max_descriptor_set_storage_images > 0 && l.max_descriptor_set_storage_images >= p.
        max_descriptor_set_storage_images)
        score += 1;
    if (p.max_descriptor_set_input_attachments > 0 && l.max_descriptor_set_input_attachments >= p.
        max_descriptor_set_input_attachments)
        score += 1;
    if (p.max_vertex_input_attributes > 0 && l.max_vertex_input_attributes >= p.max_vertex_input_attributes)
        score
            += 1;
    if (p.max_vertex_input_bindings > 0 && l.max_vertex_input_bindings >= p.max_vertex_input_bindings)
        score += 1;
    if (p.max_vertex_input_attribute_offset > 0 && l.max_vertex_input_attribute_offset >= p.
        max_vertex_input_attribute_offset)
        score += 1;
    if (p.max_vertex_input_binding_stride > 0 && l.max_vertex_input_binding_stride >= p.
        max_vertex_input_binding_stride)
        score += 1;
    if (p.max_vertex_output_components > 0 && l.max_vertex_output_components >= p.max_vertex_output_components)
        score += 1;
    if (p.max_tessellation_generation_level > 0 && l.max_tessellation_generation_level >= p.
        max_tessellation_generation_level)
        score += 1;
    if (p.max_tessellation_patch_size > 0 && l.max_tessellation_patch_size >= p.max_tessellation_patch_size)
        score
            += 1;
    if (p.max_tessellation_control_per_vertex_input_components > 0 && l.
        max_tessellation_control_per_vertex_input_components >= p.
        max_tessellation_control_per_vertex_input_components)
        score += 1;
    if (p.max_tessellation_control_per_vertex_output_components > 0 && l.
        max_tessellation_control_per_vertex_output_components >= p.
        max_tessellation_control_per_vertex_output_components)
        score += 1;
    if (p.max_tessellation_control_per_patch_output_components > 0 && l.
        max_tessellation_control_per_patch_output_components >= p.
        max_tessellation_control_per_patch_output_components)
        score += 1;
    if (p.max_tessellation_control_total_output_components > 0 && l.max_tessellation_control_total_output_components
        >= p.max_tessellation_control_total_output_components)
        score += 1;
    if (p.max_tessellation_evaluation_input_components > 0 && l.max_tessellation_evaluation_input_components >= p.
        max_tessellation_evaluation_input_components)
        score += 1;
    if (p.max_tessellation_evaluation_output_components > 0 && l.max_tessellation_evaluation_output_components >= p.
        max_tessellation_evaluation_output_components)
        score += 1;
    if (p.max_geometry_shader_invocations > 0 && l.max_geometry_shader_invocations >= p.
        max_geometry_shader_invocations)
        score += 1;
    if (p.max_geometry_input_components > 0 && l.max_geometry_input_components >= p.max_geometry_input_components)
        score += 1;
    if (p.max_geometry_output_components > 0 && l.max_geometry_output_components >= p.
        max_geometry_output_components)
        score += 1;
    if (p.max_geometry_output_vertices > 0 && l.max_geometry_output_vertices >= p.max_geometry_output_vertices)
        score += 1;
    if (p.max_geometry_total_output_components > 0 && l.max_geometry_total_output_components >= p.
        max_geometry_total_output_components)
        score += 1;
    if (p.max_fragment_input_components > 0 && l.max_fragment_input_components >= p.max_fragment_input_components)
        score += 1;
    if (p.max_fragment_output_attachments > 0 && l.max_fragment_output_attachments >= p.
        max_fragment_output_attachments)
        score += 1;
    if (p.max_fragment_dual_src_attachments > 0 && l.max_fragment_dual_src_attachments >= p.
        max_fragment_dual_src_attachments)
        score += 1;
    if (p.max_fragment_combined_output_resources > 0 && l.max_fragment_combined_output_resources >= p.
        max_fragment_combined_output_resources)
        score += 1;
    if (p.max_compute_shared_memory_size > 0 && l.max_compute_shared_memory_size >= p.
        max_compute_shared_memory_size)
        score += 1;
    for (size_t i = 0; i < 3; i++)
    {
        if (p.max_compute_work_group_count[i] > 0 && l.max_compute_work_group_count[i] >= p.
            max_compute_work_group_count[i])
            score += 1;
    }
    if (p.max_compute_work_group_invocations > 0 && l.max_compute_work_group_invocations >= p.
        max_compute_work_group_invocations)
        score += 1;
    for (size_t i = 0; i < 3; i++)
    {
        if (p.max_compute_work_group_size[i] > 0 && l.max_compute_work_group_size[i] >= p.
            max_compute_work_group_size[i])
            score += 1;
    }
    if (p.sub_pixel_precision_bits > 0 && l.sub_pixel_precision_bits >= p.sub_pixel_precision_bits)
        score += 1;
    if (p.sub_texel_precision_bits > 0 && l.sub_texel_precision_bits >= p.sub_texel_precision_bits)
        score += 1;
    if (p.mipmap_precision_bits > 0 && l.mipmap_precision_bits >= p.mipmap_precision_bits)
        score += 1;
    if (p.max_draw_indexed_index_value > 0 && l.max_draw_indexed_index_value >= p.max_draw_indexed_index_value)
        score += 1;
    if (p.max_draw_indirect_count > 0 && l.max_draw_indirect_count >= p.max_draw_indirect_count)
        score += 1;
    if (p.max_sampler_lod_bias != 0.0f && l.max_sampler_lod_bias >= p.max_sampler_lod_bias)
        score += 1;
    if (p.max_sampler_anisotropy != 0.0f && l.max_sampler_anisotropy >= p.max_sampler_anisotropy)
        score += 1;
    if (p.max_viewports > 0 && l.max_viewports >= p.max_viewports)
        score += 1;
    for (size_t i = 0; i < 2; i++)
    {
        if (p.max_viewport_dimensions[i] > 0 && l.max_viewport_dimensions[i] >= p.max_viewport_dimensions[i])
            score
                += 1;
    }
    for (size_t i = 0; i < 2; i++)
    {
        if (p.viewport_bounds_range[i] != 0.0f)
        {
            if ((l.viewport_bounds_range[i] < 0.0f && l.viewport_bounds_range[i] <= p.viewport_bounds_range[i])
                || (l.viewport_bounds_range[i] > 0.0f && l.viewport_bounds_range[i] >= p.viewport_bounds_range[i]))
                score += 1;
        }
    }
    if (p.viewport_sub_pixel_bits > 0 && l.viewport_sub_pixel_bits >= p.viewport_sub_pixel_bits)
        score += 1;
    if (p.min_memory_map_alignment > 0 && l.min_memory_map_alignment >= p.min_memory_map_alignment)
        score += 1;
    if (p.min_texel_buffer_offset_alignment > 0 && l.min_texel_buffer_offset_alignment >= p.
        min_texel_buffer_offset_alignment)
        score += 1;
    if (p.min_uniform_buffer_offset_alignment > 0 && l.min_uniform_buffer_offset_alignment >= p.
        min_uniform_buffer_offset_alignment)
        score += 1;
    if (p.min_storage_buffer_offset_alignment > 0 && l.min_storage_buffer_offset_alignment >= p.
        min_storage_buffer_offset_alignment)
        score += 1;
    if (p.min_texel_offset != 0)
    {
        if ((l.min_texel_offset < 0 && l.min_texel_offset <= p.min_texel_offset)
            || (l.min_texel_offset > 0 && l.min_texel_offset >= p.min_texel_offset))
            score += 1;
    }
    if (p.max_texel_offset > 0 && l.max_texel_offset >= p.max_texel_offset)
        score += 1;
    if (p.min_texel_gather_offset != 0)
    {
        if ((l.min_texel_gather_offset < 0 && l.min_texel_gather_offset <= p.min_texel_gather_offset)
            || (l.min_texel_gather_offset > 0 && l.min_texel_gather_offset >= p.min_texel_gather_offset))
            score += 1;
    }
    if (p.max_texel_gather_offset > 0 && l.max_texel_gather_offset >= p.max_texel_gather_offset)
        score += 1;
    if (p.min_interpolation_offset != 0.0f)
    {
        if ((l.min_interpolation_offset < 0.0f && l.min_interpolation_offset <= p.min_interpolation_offset)
            || (l.min_interpolation_offset > 0.0f && l.min_interpolation_offset >= p.min_interpolation_offset))
            score += 1;
    }
    if (p.max_interpolation_offset != 0.0f)
    {
        if ((l.max_interpolation_offset < 0.0f && l.max_interpolation_offset <= p.max_interpolation_offset)
            || (l.max_interpolation_offset > 0.0f && l.max_interpolation_offset >= p.max_interpolation_offset))
            score += 1;
    }
    if (p.sub_pixel_interpolation_offset_bits > 0 && l.sub_pixel_interpolation_offset_bits >= p.
        sub_pixel_interpolation_offset_bits)
        score += 1;
    if (p.max_framebuffer_width > 0 && l.max_framebuffer_width >= p.max_framebuffer_width)
        score += 1;
    if (p.max_framebuffer_height > 0 && l.max_framebuffer_height >= p.max_framebuffer_height)
        score += 1;
    if (p.max_framebuffer_layers > 0 && l.max_framebuffer_layers >= p.max_framebuffer_layers)
        score += 1;
    if (p.framebuffer_color_sample_counts != SampleCountFlags::NONE && l.framebuffer_color_sample_counts >= p.
        framebuffer_color_sample_counts)
        score += 1;
    if (p.framebuffer_depth_sample_counts != SampleCountFlags::NONE && l.framebuffer_depth_sample_counts >= p.
        framebuffer_depth_sample_counts)
        score += 1;
    if (p.framebuffer_stencil_sample_counts != SampleCountFlags::NONE && l.framebuffer_stencil_sample_counts >= p.
        framebuffer_stencil_sample_counts)
        score += 1;
    if (p.framebuffer_no_attachments_sample_counts != SampleCountFlags::NONE && l.
        framebuffer_no_attachments_sample_counts >= p.framebuffer_no_attachments_sample_counts)
        score += 1;
    if (p.max_color_attachments > 0 && l.max_color_attachments >= p.max_color_attachments)
        score += 1;
    if (p.sampled_image_color_sample_counts != SampleCountFlags::NONE && l.sampled_image_color_sample_counts >= p.
        sampled_image_color_sample_counts)
        score += 1;
    if (p.sampled_image_integer_sample_counts != SampleCountFlags::NONE && l.sampled_image_integer_sample_counts >=
        p.sampled_image_integer_sample_counts)
        score += 1;
    if (p.sampled_image_depth_sample_counts != SampleCountFlags::NONE && l.sampled_image_depth_sample_counts >= p.
        sampled_image_depth_sample_counts)
        score += 1;
    if (p.sampled_image_stencil_sample_counts != SampleCountFlags::NONE && l.sampled_image_stencil_sample_counts >=
        p.sampled_image_stencil_sample_counts)
        score += 1;
    if (p.storage_image_sample_counts != SampleCountFlags::NONE && l.storage_image_sample_counts >= p.
        storage_image_sample_counts)
        score += 1;
    if (p.max_sample_mask_words > 0 && l.max_sample_mask_words >= p.max_sample_mask_words)
        score += 1;
    if (p.timestamp_compute_and_graphics && l.timestamp_compute_and_graphics == p.timestamp_compute_and_graphics)
        score += 1;
    if (p.timestamp_period != 0.0f && l.timestamp_period >= p.timestamp_period)
        score += 1;
    if (p.max_clip_distances > 0 && l.max_clip_distances >= p.max_clip_distances)
        score += 1;
    if (p.max_cull_distances > 0 && l.max_cull_distances >= p.max_cull_distances)
        score += 1;
    if (p.max_combined_clip_and_cull_distances > 0 && l.max_combined_clip_and_cull_distances >= p.
        max_combined_clip_and_cull_distances)
        score += 1;
    if (p.discrete_queue_priorities > 0 && l.discrete_queue_priorities >= p.discrete_queue_priorities)
        score += 1;
    for (size_t i = 0; i < 2; i++)
    {
        if (p.point_size_range[i] > 0.0f && l.point_size_range[i] >= p.point_size_range[i])
            score += 1;
    }
    for (size_t i = 0; i < 2; i++)
    {
        if (p.line_width_range[i] > 0.0f && l.line_width_range[i] >= p.line_width_range[i])
            score += 1;
    }
    if (p.point_size_granularity != 0.0f && l.point_size_granularity >= p.point_size_granularity)
        score += 1;
    if (p.line_width_granularity != 0.0f && l.line_width_granularity >= p.line_width_granularity)
        score += 1;
    if (p.strict_lines && l.strict_lines == p.strict_lines)
        score += 1;
    if (p.standard_sample_locations && l.standard_sample_locations == p.standard_sample_locations)
        score += 1;
    if (p.optimal_buffer_copy_offset_alignment > 0 && l.optimal_buffer_copy_offset_alignment >= p.
        optimal_buffer_copy_offset_alignment)
        score += 1;
    if (p.optimal_buffer_copy_row_pitch_alignment > 0 && l.optimal_buffer_copy_row_pitch_alignment >= p.
        optimal_buffer_copy_row_pitch_alignment)
        score += 1;
    if (p.non_coherent_atom_size > 0 && l.non_coherent_atom_size >= p.non_coherent_atom_size)
        score += 1;
    // bindless limits
    if (p.max_update_after_bind_descriptors_in_all_pools > 0 && l.max_update_after_bind_descriptors_in_all_pools >=
        p.max_update_after_bind_descriptors_in_all_pools)
        score += 1;
    if (p.shader_uniform_buffer_array_non_uniform_indexing_native && l.
        shader_uniform_buffer_array_non_uniform_indexing_native == p.
        shader_uniform_buffer_array_non_uniform_indexing_native)
        score += 1;
    if (p.shader_sampled_image_array_non_uniform_indexing_native && l.
        shader_sampled_image_array_non_uniform_indexing_native == p.
        shader_sampled_image_array_non_uniform_indexing_native)
        score += 1;
    if (p.shader_storage_buffer_array_non_uniform_indexing_native && l.
        shader_storage_buffer_array_non_uniform_indexing_native == p.
        shader_storage_buffer_array_non_uniform_indexing_native)
        score += 1;
    if (p.shader_storage_image_array_non_uniform_indexing_native && l.
        shader_storage_image_array_non_uniform_indexing_native == p.
        shader_storage_image_array_non_uniform_indexing_native)
        score += 1;
    if (p.shader_input_attachment_array_non_uniform_indexing_native && l.
        shader_input_attachment_array_non_uniform_indexing_native == p.
        shader_input_attachment_array_non_uniform_indexing_native)
        score += 1;
    if (p.robust_buffer_access_update_after_bind && l.robust_buffer_access_update_after_bind == p.
        robust_buffer_access_update_after_bind)
        score += 1;
    if (p.quad_divergent_implicit_lod && l.quad_divergent_implicit_lod == p.quad_divergent_implicit_lod)
        score += 1;
    if (p.max_per_stage_descriptor_update_after_bind_samplers > 0 && l.
        max_per_stage_descriptor_update_after_bind_samplers >= p.
        max_per_stage_descriptor_update_after_bind_samplers)
        score += 1;
    if (p.max_per_stage_descriptor_update_after_bind_uniform_buffers > 0 && l.
        max_per_stage_descriptor_update_after_bind_uniform_buffers >= p.
        max_per_stage_descriptor_update_after_bind_uniform_buffers)
        score += 1;
    if (p.max_per_stage_descriptor_update_after_bind_storage_buffers > 0 && l.
        max_per_stage_descriptor_update_after_bind_storage_buffers >= p.
        max_per_stage_descriptor_update_after_bind_storage_buffers)
        score += 1;
    if (p.max_per_stage_descriptor_update_after_bind_sampled_images > 0 && l.
        max_per_stage_descriptor_update_after_bind_sampled_images >= p.
        max_per_stage_descriptor_update_after_bind_sampled_images)
        score += 1;
    if (p.max_per_stage_descriptor_update_after_bind_storage_images > 0 && l.
        max_per_stage_descriptor_update_after_bind_storage_images >= p.
        max_per_stage_descriptor_update_after_bind_storage_images)
        score += 1;
    if (p.max_per_stage_descriptor_update_after_bind_input_attachments > 0 && l.
        max_per_stage_descriptor_update_after_bind_input_attachments >= p.
        max_per_stage_descriptor_update_after_bind_input_attachments)
        score += 1;
    if (p.max_per_stage_update_after_bind_resources > 0 && l.max_per_stage_update_after_bind_resources >= p.
        max_per_stage_update_after_bind_resources)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_samplers > 0 && l.max_descriptor_set_update_after_bind_samplers >= p.
        max_descriptor_set_update_after_bind_samplers)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_uniform_buffers > 0 && l.
        max_descriptor_set_update_after_bind_uniform_buffers >= p.
        max_descriptor_set_update_after_bind_uniform_buffers)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_uniform_buffers_dynamic > 0 && l.
        max_descriptor_set_update_after_bind_uniform_buffers_dynamic >= p.
        max_descriptor_set_update_after_bind_uniform_buffers_dynamic)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_storage_buffers > 0 && l.
        max_descriptor_set_update_after_bind_storage_buffers >= p.
        max_descriptor_set_update_after_bind_storage_buffers)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_storage_buffers_dynamic > 0 && l.
        max_descriptor_set_update_after_bind_storage_buffers_dynamic >= p.
        max_descriptor_set_update_after_bind_storage_buffers_dynamic)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_sampled_images > 0 && l.
        max_descriptor_set_update_after_bind_sampled_images >= p.
        max_descriptor_set_update_after_bind_sampled_images)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_storage_images > 0 && l.
        max_descriptor_set_update_after_bind_storage_images >= p.
        max_descriptor_set_update_after_bind_storage_images)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_input_attachments > 0 && l.
        max_descriptor_set_update_after_bind_input_attachments >= p.
        max_descriptor_set_update_after_bind_input_attachments)
        score += 1;
    // Acceleration Structure limits
    if (p.max_geometry_count > 0 && l.max_geometry_count >= p.max_geometry_count)
        score += 1;
    if (p.max_instance_count > 0 && l.max_instance_count >= p.max_instance_count)
        score += 1;
    if (p.max_primitive_count > 0 && l.max_primitive_count >= p.max_primitive_count)
        score += 1;
    if (p.max_per_stage_descriptor_acceleration_structures > 0 && l.max_per_stage_descriptor_acceleration_structures
        >= p.max_per_stage_descriptor_acceleration_structures)
        score += 1;
    if (p.max_per_stage_descriptor_update_after_bind_acceleration_structures > 0 && l.
        max_per_stage_descriptor_update_after_bind_acceleration_structures >= p.
        max_per_stage_descriptor_update_after_bind_acceleration_structures)
        score += 1;
    if (p.max_descriptor_set_acceleration_structures > 0 && l.max_descriptor_set_acceleration_structures >= p.
        max_descriptor_set_acceleration_structures)
        score += 1;
    if (p.max_descriptor_set_update_after_bind_acceleration_structures > 0 && l.
        max_descriptor_set_update_after_bind_acceleration_structures >= p.
        max_descriptor_set_update_after_bind_acceleration_structures)
        score += 1;
    if (p.min_acceleration_structure_scratch_offset_alignment > 0 && l.
        min_acceleration_structure_scratch_offset_alignment >= p.
        min_acceleration_structure_scratch_offset_alignment)
        score += 1;
    // Raytracing limits
    if (p.shader_group_handle_size > 0 && l.shader_group_handle_size >= p.shader_group_handle_size)
        score += 1;
    if (p.max_ray_recursion_depth > 0 && l.max_ray_recursion_depth >= p.max_ray_recursion_depth)
        score += 1;
    if (p.max_shader_group_stride > 0 && l.max_shader_group_stride >= p.max_shader_group_stride)
        score += 1;
    if (p.shader_group_base_alignment > 0 && l.shader_group_base_alignment >= p.shader_group_base_alignment)
        score
            += 1;
    if (p.shader_group_handle_capture_replay_size > 0 && l.shader_group_handle_capture_replay_size >= p.
        shader_group_handle_capture_replay_size)
        score += 1;
    if (p.max_ray_dispatch_invocation_count > 0 && l.max_ray_dispatch_invocation_count >= p.
        max_ray_dispatch_invocation_count)
        score += 1;
    if (p.shader_group_handle_alignment > 0 && l.shader_group_handle_alignment >= p.shader_group_handle_alignment)
        score += 1;
    if (p.max_ray_hit_attribute_size > 0 && l.max_ray_hit_attribute_size >= p.max_ray_hit_attribute_size)
        score +=
            1;
    return score;
}

/**
 * Returns a score for the given physical device based on the specified GPU selection preference.
 * @param device Vulkan Physical Device handle.
 * @param surface Vulkan Surface handle. (Pass `nullptr` if there's none).
 * @param preference GPU type, features, limits, etc. used to score the given device.
 * @return integer number indicating the score for the device.
 */
inline uint32_t getGPUScore(const vk::PhysicalDevice device, const vk::SurfaceKHR surface,
                            const GPUSelectionPreference &preference)
{
    uint32_t score = 0;
    if (preference.type != GPUType::UNKNOWN &&
        preference.type == getGPUType(device.getProperties().deviceType))
    {
        score += 1000;
    }

    const GPUFeatures features = getGPUFeatures(device);
    score += getGPUFeatureScore(features, preference.features);
    const GPULimits limits = getGPULimits(device);
    score += getGPULimitScore(limits, preference.limits);

    std::vector<QueueFamily> queue_families = getQueueFamilies(device, surface);
    auto all_family_flags = QueueFamilyFlags::NONE;
    bool supports_async_compute = false;
    bool supports_async_transfer = false;
    for (const auto &q_f : queue_families)
    {
        if (q_f.supportsOnlyCompute())
        {
            supports_async_compute = true;
        }
        if (q_f.supportsOnlyTransfer())
        {
            supports_async_transfer = true;
        }
        all_family_flags = all_family_flags | q_f.support_flags;
    }

    // Check if all the preferred queue types are present in the device.
    if (preference.queue_types != QueueFamilyFlags::NONE && (all_family_flags & preference.queue_types) ==
        preference.queue_types)
    {
        score += 1000;
    }
    // If there are multiple queue families, check if there's a dedicated compute queue family.
    if (preference.dedicated_compute_queue && supports_async_compute)
    {
        score += 1000;
    }
    // If there are multiple queue families, check if there's a dedicated transfer queue family.
    if (preference.dedicated_transfer_queue && supports_async_transfer)
    {
        score += 1000;
    }

    return score;
}


/**
 * Returns physical device features which are used when enabling features when creating a
 * logical device.
 * @param f The features to be enabled on logical device creation.
 * @return A Vulkan physical device features.
 */
inline vk::PhysicalDeviceFeatures2 getEnabledFeatures(const GPUFeatures &f)
{
    vk::PhysicalDeviceRayTracingPipelineFeaturesKHR raytracing_features = {};
    vk::PhysicalDeviceAccelerationStructureFeaturesKHR accel_struct_features = {};
    accel_struct_features.pNext = &raytracing_features;
    vk::PhysicalDeviceDescriptorIndexingFeatures bindless_features = {};
    bindless_features.pNext = &accel_struct_features;
    vk::PhysicalDeviceShaderAtomicInt64Features atomic64_features = {};
    atomic64_features.pNext = &bindless_features;
    vk::PhysicalDeviceShaderFloat16Int8Features f16_features = {};
    f16_features.pNext = &atomic64_features;
    vk::PhysicalDeviceFeatures2 features = {};
    features.pNext = &f16_features;

    // Base Features
    if (f.robust_buffer_access)
        features.features.robustBufferAccess = true;
    if (f.full_draw_index_uint32)
        features.features.fullDrawIndexUint32 = true;
    if (f.image_cube_array)
        features.features.imageCubeArray = true;
    if (f.independent_blend)
        features.features.independentBlend = true;
    if (f.geometry_shader)
        features.features.geometryShader = true;
    if (f.tessellation_shader)
        features.features.tessellationShader = true;
    if (f.sample_rate_shading)
        features.features.sampleRateShading = true;
    if (f.dual_src_blend)
        features.features.dualSrcBlend = true;
    if (f.logic_op)
        features.features.logicOp = true;
    if (f.multi_draw_indirect)
        features.features.multiDrawIndirect = true;
    if (f.draw_indirect_first_instance)
        features.features.drawIndirectFirstInstance = true;
    if (f.depth_clamp)
        features.features.depthClamp = true;
    if (f.depth_bias_clamp)
        features.features.depthBiasClamp = true;
    if (f.fill_mode_non_solid)
        features.features.fillModeNonSolid = true;
    if (f.depth_bounds)
        features.features.depthBounds = true;
    if (f.wide_lines)
        features.features.wideLines = true;
    if (f.large_points)
        features.features.largePoints = true;
    if (f.alpha_to_one)
        features.features.alphaToOne = true;
    if (f.multi_viewport)
        features.features.multiViewport = true;
    if (f.sampler_anisotropy)
        features.features.samplerAnisotropy = true;
    if (f.texture_compression_ETC2)
        features.features.textureCompressionETC2 = true;
    if (f.texture_compression_ASTCLDR)
        features.features.textureCompressionASTC_LDR = true;
    if (f.texture_compression_BC)
        features.features.textureCompressionBC = true;
    if (f.occlusion_query_precise)
        features.features.occlusionQueryPrecise = true;
    if (f.pipeline_statistics_query)
        features.features.pipelineStatisticsQuery = true;
    if (f.vertex_pipeline_stores_and_atomics)
        features.features.vertexPipelineStoresAndAtomics = true;
    if (f.fragment_stores_and_atomics)
        features.features.fragmentStoresAndAtomics = true;
    if (f.shader_tessellation_and_geometry_point_size)
        features.features.shaderTessellationAndGeometryPointSize =
            true;
    if (f.shader_image_gather_extended)
        features.features.shaderImageGatherExtended = true;
    if (f.shader_storage_image_extended_formats)
        features.features.shaderStorageImageExtendedFormats = true;
    if (f.shader_storage_image_multisample)
        features.features.shaderStorageImageMultisample = true;
    if (f.shader_storage_image_read_without_format)
        features.features.shaderStorageImageReadWithoutFormat = true;
    if (f.shader_storage_image_write_without_format)
        features.features.shaderStorageImageWriteWithoutFormat = true;
    if (f.shader_uniform_buffer_array_dynamic_indexing)
        features.features.shaderUniformBufferArrayDynamicIndexing =
            true;
    if (f.shader_sampled_image_array_dynamic_indexing)
        features.features.shaderSampledImageArrayDynamicIndexing =
            true;
    if (f.shader_storage_buffer_array_dynamic_indexing)
        features.features.shaderStorageBufferArrayDynamicIndexing =
            true;
    if (f.shader_storage_image_array_dynamic_indexing)
        features.features.shaderStorageImageArrayDynamicIndexing =
            true;
    if (f.shader_clip_distance)
        features.features.shaderClipDistance = true;
    if (f.shader_cull_distance)
        features.features.shaderCullDistance = true;
    if (f.shader_float64)
        features.features.shaderFloat64 = true;
    if (f.shader_int64)
        features.features.shaderInt64 = true;
    if (f.shader_int16)
        features.features.shaderInt16 = true;
    if (f.shader_resource_residency)
        features.features.shaderResourceResidency = true;
    if (f.shader_resource_min_lod)
        features.features.shaderResourceMinLod = true;
    if (f.sparse_binding)
        features.features.sparseBinding = true;
    if (f.sparse_residency_buffer)
        features.features.sparseResidencyBuffer = true;
    if (f.sparse_residency_image2D)
        features.features.sparseResidencyImage2D = true;
    if (f.sparse_residency_image3D)
        features.features.sparseResidencyImage3D = true;
    if (f.sparse_residency2_samples)
        features.features.sparseResidency2Samples = true;
    if (f.sparse_residency4_samples)
        features.features.sparseResidency4Samples = true;
    if (f.sparse_residency8_samples)
        features.features.sparseResidency8Samples = true;
    if (f.sparse_residency16_samples)
        features.features.sparseResidency16Samples = true;
    if (f.sparse_residency_aliased)
        features.features.sparseResidencyAliased = true;
    if (f.variable_multisample_rate)
        features.features.variableMultisampleRate = true;
    if (f.inherited_queries)
        features.features.inheritedQueries = true;
    // 16-bit float 8-bit int features
    if (f.shader_float16)
        f16_features.shaderFloat16 = true;
    if (f.shader_int8)
        f16_features.shaderInt8 = true;
    // int 64-bit atomic features
    if (f.shader_buffer_int64_atomics)
        atomic64_features.shaderBufferInt64Atomics = true;
    if (f.shader_shared_int64_atomics)
        atomic64_features.shaderSharedInt64Atomics = true;
    // Bindless support features
    if (f.shader_input_attachment_array_dynamic_indexing)
        bindless_features.
            shaderInputAttachmentArrayDynamicIndexing = true;
    if (f.shader_uniform_texel_buffer_array_dynamic_indexing)
        bindless_features.shaderUniformTexelBufferArrayDynamicIndexing = true;
    if (f.shader_storage_texel_buffer_array_dynamic_indexing)
        bindless_features.
            shaderStorageTexelBufferArrayNonUniformIndexing = true;
    if (f.shader_uniform_buffer_array_non_uniform_indexing)
        bindless_features.
            shaderUniformBufferArrayNonUniformIndexing = true;
    if (f.shader_sampled_image_array_non_uniform_indexing)
        bindless_features.
            shaderSampledImageArrayNonUniformIndexing = true;
    if (f.shader_storage_buffer_array_non_uniform_indexing)
        bindless_features.
            shaderStorageBufferArrayNonUniformIndexing = true;
    if (f.shader_storage_image_array_non_uniform_indexing)
        bindless_features.
            shaderStorageImageArrayNonUniformIndexing = true;
    if (f.shader_input_attachment_array_non_uniform_indexing)
        bindless_features.
            shaderInputAttachmentArrayNonUniformIndexing = true;
    if (f.shader_uniform_texel_buffer_array_non_uniform_indexing)
        bindless_features.
            shaderUniformTexelBufferArrayNonUniformIndexing = true;
    if (f.shader_storage_texel_buffer_array_non_uniform_indexing)
        bindless_features.
            shaderStorageTexelBufferArrayNonUniformIndexing = true;
    if (f.descriptor_binding_uniform_buffer_update_after_bind)
        bindless_features.
            descriptorBindingUniformBufferUpdateAfterBind = true;
    if (f.descriptor_binding_sampled_image_update_after_bind)
        bindless_features.
            descriptorBindingSampledImageUpdateAfterBind = true;
    if (f.descriptor_binding_storage_image_update_after_bind)
        bindless_features.
            descriptorBindingStorageImageUpdateAfterBind = true;
    if (f.descriptor_binding_storage_buffer_update_after_bind)
        bindless_features.
            descriptorBindingStorageBufferUpdateAfterBind = true;
    if (f.descriptor_binding_uniform_texel_buffer_update_after_bind)
        bindless_features.
            descriptorBindingUniformTexelBufferUpdateAfterBind = true;
    if (f.descriptor_binding_storage_texel_buffer_update_after_bind)
        bindless_features.
            descriptorBindingStorageTexelBufferUpdateAfterBind = true;
    if (f.descriptor_binding_update_unused_while_pending)
        bindless_features.
            descriptorBindingUpdateUnusedWhilePending = true;
    if (f.descriptor_binding_partially_bound)
        bindless_features.descriptorBindingPartiallyBound = true;
    if (f.descriptor_binding_variable_descriptor_count)
        bindless_features
            .descriptorBindingVariableDescriptorCount = true;
    if (f.runtime_descriptor_array)
        bindless_features.runtimeDescriptorArray = true;
    // Acceleration Structure features
    if (f.acceleration_structure)
        accel_struct_features.accelerationStructure = true;
    if (f.acceleration_structure_capture_replay)
        accel_struct_features.accelerationStructureCaptureReplay = true;
    if (f.acceleration_structure_indirect_build)
        accel_struct_features.accelerationStructureIndirectBuild = true;
    if (f.acceleration_structure_host_commands)
        accel_struct_features.accelerationStructureHostCommands = true;
    if (f.descriptor_binding_acceleration_structure_update_after_bind)
        accel_struct_features.
            descriptorBindingAccelerationStructureUpdateAfterBind = true;
    // Raytracing features
    if (f.ray_tracing_pipeline)
        raytracing_features.rayTracingPipeline = true;
    if (f.ray_tracing_pipeline_shader_group_handle_capture_replay)
        raytracing_features.
            rayTracingPipelineShaderGroupHandleCaptureReplay = true;
    if (f.ray_tracing_pipeline_shader_group_handle_capture_replay_mixed)
        raytracing_features.
            rayTracingPipelineShaderGroupHandleCaptureReplayMixed = true;
    if (f.ray_tracing_pipeline_trace_rays_indirect)
        raytracing_features.rayTracingPipelineTraceRaysIndirect = true;
    if (f.ray_traversal_primitive_culling)
        raytracing_features.rayTraversalPrimitiveCulling = true;
    return features;
}

/**
 * Returns device extension names that need to be enabled to support given features.
 * @param f The features to be enabled on logical device creation.
 * @return A vector of device extension names.
 */
inline std::vector<const char *> getEnabledDeviceExtensions(const GPUFeatures &f)
{
    std::vector<const char *> extensions = {};
    // 16-bit float 8-bit int features
    if (f.shader_float16 || f.shader_int8)
    {
        extensions.push_back(vk::KHRShaderFloat16Int8ExtensionName);
    }
    // int 64-bit atomic features
    if (f.shader_buffer_int64_atomics || f.shader_shared_int64_atomics)
    {
        extensions.push_back(vk::KHRShaderAtomicInt64ExtensionName);
    }
    // Bindless support features
    if (f.shader_input_attachment_array_dynamic_indexing
        || f.shader_uniform_texel_buffer_array_dynamic_indexing
        || f.shader_storage_texel_buffer_array_dynamic_indexing
        || f.shader_uniform_buffer_array_non_uniform_indexing
        || f.shader_sampled_image_array_non_uniform_indexing
        || f.shader_storage_buffer_array_non_uniform_indexing
        || f.shader_storage_image_array_non_uniform_indexing
        || f.shader_input_attachment_array_non_uniform_indexing
        || f.shader_uniform_texel_buffer_array_non_uniform_indexing
        || f.shader_storage_texel_buffer_array_non_uniform_indexing
        || f.descriptor_binding_uniform_buffer_update_after_bind
        || f.descriptor_binding_sampled_image_update_after_bind
        || f.descriptor_binding_storage_image_update_after_bind
        || f.descriptor_binding_storage_buffer_update_after_bind
        || f.descriptor_binding_uniform_texel_buffer_update_after_bind
        || f.descriptor_binding_storage_texel_buffer_update_after_bind
        || f.descriptor_binding_update_unused_while_pending
        || f.descriptor_binding_partially_bound
        || f.descriptor_binding_variable_descriptor_count
        || f.runtime_descriptor_array)
    {
        extensions.push_back(vk::EXTDescriptorIndexingExtensionName);
    }
    // Acceleration Structure features
    if (f.acceleration_structure
        || f.acceleration_structure_capture_replay
        || f.acceleration_structure_indirect_build
        || f.acceleration_structure_host_commands
        || f.descriptor_binding_acceleration_structure_update_after_bind)
    {
        extensions.push_back(vk::KHRAccelerationStructureExtensionName);
    }
    // Raytracing features
    if (f.ray_tracing_pipeline
        || f.ray_tracing_pipeline_shader_group_handle_capture_replay
        || f.ray_tracing_pipeline_shader_group_handle_capture_replay_mixed
        || f.ray_tracing_pipeline_trace_rays_indirect
        || f.ray_traversal_primitive_culling)
    {
        extensions.push_back(vk::KHRRayTracingPipelineExtensionName);
    }
    return extensions;
}

inline vk::PresentModeKHR getPresentMode(const PresentMode mode)
{
    switch (mode)
    {
    case PresentMode::MAILBOX:
        return vk::PresentModeKHR::eMailbox;
    case PresentMode::FIFO:
        return vk::PresentModeKHR::eFifo;
    case PresentMode::FIFO_RELAXED:
        return vk::PresentModeKHR::eFifoRelaxed;
    case PresentMode::IMMEDIATE:
    default:
        return vk::PresentModeKHR::eImmediate;
    }
}
}
#endif // KIRANA_RENDERER_HELPERS_VULKAN_HPP