// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_RENDERER_PIPELINE_RENDER_HPP
#define KIRANA_RENDERER_PIPELINE_RENDER_HPP

#include "pipeline_layout.hpp"
#include "shader.hpp"

namespace kirana::renderer
{

enum class VertexInputRate: uint8_t
{
    VERTEX = 0u,
    INSTANCE = 1u,
};

enum class PrimitiveTopology: uint8_t
{
    POINT_LIST = 0u,
    LINE_LIST = 1u,
    LINE_STRIP = 2u,
    TRIANGLE_LIST = 3u,
    TRIANGLE_STRIP = 4u,
    TRIANGLE_FAN = 5u,
    LINE_LIST_WITH_ADJACENCY = 6u,
    LINE_STRIP_WITH_ADJACENCY = 7u,
    TRIANGLE_LIST_WITH_ADJACENCY = 8u,
    TRIANGLE_STRIP_WITH_ADJACENCY = 9u,
    PATCH_LIST = 10u,
};

enum class FillMode: uint8_t
{
    FILL = 0u,
    LINE = 1u,
    POINT = 2u,
};

enum class CullMode: uint8_t
{
    NONE = 0u,
    FRONT = 1u,
    BACK = 2u,
    ALL = 3u,
};

enum class PrimitiveWindingOrder: uint8_t
{
    COUNTER_CLOCKWISE = 0u,
    CLOCKWISE = 1u,
};

enum class CompareOperation: uint8_t
{
    NEVER = 0u,
    LESS = 1u,
    EQUAL = 2u,
    LESS_OR_EQUAL = 3u,
    GREATER = 4u,
    NOT_EQUAL = 5u,
    GREATER_OR_EQUAL = 6u,
    ALWAYS = 7u,
};

enum class StencilOperation: uint8_t
{
    KEEP = 0,
    ZERO = 1,
    REPLACE = 2,
    INCREMENT_AND_CLAMP = 3,
    DECREMENT_AND_CLAMP = 4,
    INVERT = 5,
    INCREMENT_AND_WRAP = 6,
    DECREMENT_AND_WRAP = 7,
};

enum class ColorComponentFlags: uint8_t
{
    NONE = 0x00u,
    R = 0x01u,
    G = 0x02u,
    B = 0x04u,
    A = 0x08u,
    ALL = 0x0Fu,
};

enum class BlendFactor: uint8_t
{
    ZERO = 0u,
    ONE = 1u,
    SRC_COLOR = 2u,
    ONE_MINUS_SRC_COLOR = 3u,
    DST_COLOR = 4u,
    ONE_MINUS_DST_COLOR = 5u,
    SRC_ALPHA = 6u,
    ONE_MINUS_SRC_ALPHA = 7u,
    DST_ALPHA = 8u,
    ONE_MINUS_DST_ALPHA = 9u,
    CONSTANT_COLOR = 10u,
    ONE_MINUS_CONSTANT_COLOR = 11u,
    CONSTANT_ALPHA = 12u,
    ONE_MINUS_CONSTANT_ALPHA = 13u,
    SRC_ALPHA_SATURATE = 14u,
    SRC1_COLOR = 15u,
    ONE_MINUS_SRC1_COLOR = 16u,
    SRC1_ALPHA = 17u,
    ONE_MINUS_SRC1_ALPHA = 18u,
};

enum class BlendOperation: uint8_t
{
    ADD = 0,
    SUBTRACT = 1,
    REVERSE_SUBTRACT = 2,
    MIN = 3,
    MAX = 4,
};

struct VertexBufferAttribute
{
    uint32_t offset{0};
    TextureFormat format{TextureFormat::R32G32B32A32_SFLOAT}; // vec4
};

struct VertexBufferLayout
{
    uint32_t stride{0};
    VertexInputRate input_rate{VertexInputRate::VERTEX};
    std::vector<VertexBufferAttribute> attributes{};
};

struct RasterizationState
{
    PrimitiveTopology topology{PrimitiveTopology::TRIANGLE_LIST};
    FillMode fill_mode{FillMode::FILL};
    CullMode cull_mode{CullMode::BACK};
    PrimitiveWindingOrder winding_order{PrimitiveWindingOrder::COUNTER_CLOCKWISE};
    float line_width{1.0f};
};

struct MultisampleState
{
    SampleCountFlags samples{SampleCountFlags::S_1};
    bool enable_alpha_to_coverage{false};
    bool enable_alpha_to_one{false};
};

struct StencilState
{
    StencilOperation fail_op{StencilOperation::KEEP};
    StencilOperation pass_op{StencilOperation::KEEP};
    StencilOperation depth_fail_op{StencilOperation::KEEP};
    CompareOperation compare_op{CompareOperation::NEVER};
    uint32_t compare_mask{0};
    uint32_t write_mask{0};
    uint32_t reference{0};
};

struct DepthStencilState
{
    bool test_depth{false};
    bool write_depth{false};
    CompareOperation compare_op{CompareOperation::LESS_OR_EQUAL};
    bool test_stencil{false};
    StencilState stencil_front{};
    StencilState stencil_back{};
};

struct ColorBlendState
{
    bool enable_blend{false};
    BlendFactor src_color_blend_factor{BlendFactor::ONE};
    BlendFactor dst_color_blend_factor{BlendFactor::ZERO};
    BlendOperation color_blend_op{BlendOperation::ADD};
    BlendFactor src_alpha_blend_factor{BlendFactor::ONE};
    BlendFactor dst_alpha_blend_factor{BlendFactor::ZERO};
    BlendOperation alpha_blend_op{BlendOperation::ADD};
    ColorComponentFlags color_write_mask{ColorComponentFlags::ALL};

    static ColorBlendState replace()
    {
        return ColorBlendState{};
    }

    static ColorBlendState additive()
    {
        return ColorBlendState{true, BlendFactor::SRC_ALPHA, BlendFactor::ONE, BlendOperation::ADD, BlendFactor::ONE,
                               BlendFactor::ZERO, BlendOperation::ADD, ColorComponentFlags::ALL};
    }

    static ColorBlendState alphaBlend()
    {
        return ColorBlendState{true, BlendFactor::SRC_ALPHA, BlendFactor::ONE_MINUS_SRC_ALPHA, BlendOperation::ADD,
                               BlendFactor::ONE,
                               BlendFactor::ZERO, BlendOperation::ADD, ColorComponentFlags::ALL};
    }
};

struct ColorAttachment
{
    TextureFormat format{TextureFormat::UNKNOWN};
    ColorBlendState blend_state{};
};

struct DepthStencilAttachment
{
    TextureFormat format{TextureFormat::UNKNOWN};
    DepthStencilState state{};
};

struct RenderState
{
    RasterizationState rasterization{};
    MultisampleState multisample{};
    std::vector<VertexBufferLayout> vertex_buffer_layouts{};
    std::vector<ColorAttachment> color_attachments{};
    DepthStencilAttachment depth_stencil_attachment{};
};

class PipelineRender
{
    friend class Device;

public:
    PipelineRender() = default;
    ~PipelineRender() = default;

    void destroy();

    [[nodiscard]] bool isValid() const
    {
        return m_handle != nullptr;
    }

    [[nodiscard]] const std::string &getName() const
    {
        return m_name;
    }

    [[nodiscard]] vk::Pipeline getNativeHandle() const
    {
        return m_handle;
    }

private:
    std::string m_name{};
    RenderState m_state{};

    vk::Device m_device{nullptr};

    std::vector<vk::VertexInputBindingDescription> m_vertex_input_bindings{};
    std::vector<vk::VertexInputAttributeDescription> m_vertex_input_attributes{};
    vk::PipelineVertexInputStateCreateInfo m_vertex_input_state{};

    vk::PipelineInputAssemblyStateCreateInfo m_input_assembly_state{};

    std::vector<vk::Viewport> m_viewports{};
    std::vector<vk::Rect2D> m_scissor_rects{};
    vk::PipelineViewportStateCreateInfo m_viewport_state{};

    vk::PipelineRasterizationStateCreateInfo m_rasterization_state{};
    vk::PipelineMultisampleStateCreateInfo m_multisample_state{};

    std::vector<vk::PipelineColorBlendAttachmentState> m_color_blend_attachments{};
    vk::PipelineColorBlendStateCreateInfo m_color_blend_state{};
    vk::PipelineDepthStencilStateCreateInfo m_depth_stencil_state{};

    std::vector<vk::DynamicState> m_dynamic_states{vk::DynamicState::eViewport, vk::DynamicState::eScissor};
    vk::PipelineDynamicStateCreateInfo m_dynamic_state{};

    std::vector<vk::Format> m_color_formats{};
    vk::Format m_depth_format{};
    vk::PipelineRenderingCreateInfo m_rendering_state{};

    vk::Pipeline m_handle{nullptr};

    bool init(vk::Device device, const std::string &name, const PipelineLayout &layout,
              const std::vector<Shader> &shaders, const RenderState &state = {});
};
}

#endif //KIRANA_RENDERER_PIPELINE_RENDER_HPP