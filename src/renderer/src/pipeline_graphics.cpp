// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "pipeline_graphics.hpp"

#include "helpers_vulkan.hpp"
#include "logger.hpp"

namespace kirana::renderer
{
inline vk::PipelineVertexInputStateCreateInfo getVertexInput(const std::vector<VertexBufferLayout> &vb_layouts)
{
    std::vector<vk::VertexInputBindingDescription> vertex_bindings{};
    std::vector<vk::VertexInputAttributeDescription> vertex_attribs{};
    vertex_bindings.reserve(vb_layouts.size());
    for (uint32_t i = 0; i < vb_layouts.size(); ++i)
    {
        const auto &vb_layout = vb_layouts[i];
        vertex_bindings.emplace_back(i, vb_layout.stride, static_cast<vk::VertexInputRate>(static_cast<uint8_t>(vb_layout.input_rate)));
        for (uint32_t j = 0; j < vb_layout.attributes.size(); ++j)
        {
            const auto &vb_attrib = vb_layout.attributes[j];
            vertex_attribs.emplace_back(j, i, getFormat(vb_attrib.format), vb_attrib.offset);
        }
    }

    return vk::PipelineVertexInputStateCreateInfo{
        vk::PipelineVertexInputStateCreateFlags{}, vertex_bindings, vertex_attribs};
}

inline vk::PipelineInputAssemblyStateCreateInfo getInputAssembly(const PrimitiveTopology topology)
{
    return vk::PipelineInputAssemblyStateCreateInfo{
        vk::PipelineInputAssemblyStateCreateFlags{},
        static_cast<vk::PrimitiveTopology>(static_cast<uint16_t>(topology)), false};
}

inline vk::PipelineViewportStateCreateInfo getViewportState()
{
    const std::vector viewports{vk::Viewport{}};
    const std::vector scissors{vk::Rect2D{}};
    return vk::PipelineViewportStateCreateInfo{vk::PipelineViewportStateCreateFlags{}, viewports, scissors};
}

inline vk::PipelineRasterizationStateCreateInfo getRasterState(const RasterizationState &state)
{
    const auto fill_mode = static_cast<vk::PolygonMode>(static_cast<uint8_t>(state.fill_mode));
    const auto cull_mode = static_cast<vk::CullModeFlagBits>(static_cast<uint8_t>(state.cull_mode));
    const auto front_face = static_cast<vk::FrontFace>(static_cast<uint8_t>(state.winding_order));
    return vk::PipelineRasterizationStateCreateInfo{vk::PipelineRasterizationStateCreateFlags{}, false, false,
                                                    fill_mode, cull_mode, front_face, false, 0.0f, 0.0f, 0.0f,
                                                    state.line_width};
}

inline vk::PipelineMultisampleStateCreateInfo getMultisampleState(const MultisampleState &state)
{
    const auto samples = static_cast<vk::SampleCountFlagBits>(static_cast<uint8_t>(state.samples));
    return vk::PipelineMultisampleStateCreateInfo{vk::PipelineMultisampleStateCreateFlags{}, samples, false, 1.0f,
                                                  nullptr, state.enable_alpha_to_coverage, state.enable_alpha_to_one};
}

inline vk::PipelineDepthStencilStateCreateInfo getDepthStencilState(const DepthStencilState &state)
{
    const auto getStencilOpState = [](const StencilState &stencil) {
        return vk::StencilOpState{
            static_cast<vk::StencilOp>(static_cast<uint8_t>(stencil.fail_op)),
            static_cast<vk::StencilOp>(static_cast<uint8_t>(stencil.pass_op)),
            static_cast<vk::StencilOp>(static_cast<uint8_t>(stencil.depth_fail_op)),
            static_cast<vk::CompareOp>(static_cast<uint8_t>(stencil.compare_op)),
            stencil.compare_mask, stencil.write_mask, stencil.reference};
    };

    const auto compare_op = static_cast<vk::CompareOp>(static_cast<uint8_t>(state.compare_op));
    const auto stencil_front = getStencilOpState(state.stencil_front);
    const auto stencil_back = getStencilOpState(state.stencil_back);
    return vk::PipelineDepthStencilStateCreateInfo{
        vk::PipelineDepthStencilStateCreateFlags{}, state.test_depth, state.write_depth, compare_op, false,
        state.test_stencil, stencil_front, stencil_back};
}

inline vk::PipelineColorBlendStateCreateInfo getColorBlendState(const std::vector<ColorBlendState> &attachment_states)
{
    const auto getBlendFactor = [](const BlendFactor factor) {
        return static_cast<vk::BlendFactor>(static_cast<uint8_t>(factor));
    };
    const auto getBlendOp = [](const BlendOperation op) {
        return static_cast<vk::BlendOp>(static_cast<uint8_t>(op));
    };
    std::vector<vk::PipelineColorBlendAttachmentState> blend_attachments{};
    blend_attachments.reserve(attachment_states.size());
    for (const auto &state : attachment_states)
    {

        blend_attachments.emplace_back(state.enable_blend,
                                       getBlendFactor(state.src_color_blend_factor),
                                       getBlendFactor(state.dst_color_blend_factor),
                                       getBlendOp(state.color_blend_op),
                                       getBlendFactor(state.src_alpha_blend_factor),
                                       getBlendFactor(state.dst_alpha_blend_factor),
                                       getBlendOp(state.alpha_blend_op)
            );
    }
    return vk::PipelineColorBlendStateCreateInfo{
        vk::PipelineColorBlendStateCreateFlags{}, false, vk::LogicOp::eCopy, blend_attachments};
}

inline vk::PipelineDynamicStateCreateInfo getDynamicState()
{
    std::vector dynamic_states{vk::DynamicState::eViewport, vk::DynamicState::eScissor};
    return vk::PipelineDynamicStateCreateInfo{vk::PipelineDynamicStateCreateFlags{}, dynamic_states};
}

bool PipelineGraphics::init(const vk::Device device, const PipelineLayout &layout, const std::vector<Shader> &shaders,
                            const GraphicsState &state)
{
    m_device = device;

    std::vector<vk::PipelineShaderStageCreateInfo> stage_create_infos{};
    stage_create_infos.reserve(shaders.size());
    for (const auto &s : shaders)
    {
        const auto stage = static_cast<vk::ShaderStageFlagBits>(static_cast<uint32_t>(s.getStage()));
        stage_create_infos.emplace_back(vk::PipelineShaderStageCreateFlags{}, stage, s.getNativeHandle(),
                                        s.getEntryPoint().c_str());
    }

    m_vertex_input_state = getVertexInput(state.vertex_buffer_layouts);
    m_input_assembly_state = getInputAssembly(state.rasterization.topology);
    m_viewport_state = getViewportState();
    m_rasterization_state = getRasterState(state.rasterization);
    m_multisample_state = getMultisampleState(state.multisample);
    m_depth_stencil_state = getDepthStencilState(state.depth_stencil);
    m_color_blend_state = getColorBlendState(state.attachment_blend_states);
    m_dynamic_state = getDynamicState();

    const auto create_info = vk::GraphicsPipelineCreateInfo{vk::PipelineCreateFlags{}, stage_create_infos,
                                                            &m_vertex_input_state, &m_input_assembly_state, {},
                                                            &m_viewport_state, &m_rasterization_state,
                                                            &m_multisample_state,
                                                            &m_depth_stencil_state, &m_color_blend_state,
                                                            &m_dynamic_state, layout.getNativeHandle()};

    const auto result = m_device.createGraphicsPipeline(nullptr, create_info);
    if (result.result != vk::Result::eSuccess)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to create graphics pipeline: " + vk::to_string(result.result));
        return false;
    }
    m_handle = result.value;
    return true;
}

void PipelineGraphics::destroy()
{
    if (m_device && m_handle)
    {
        m_device.destroyPipeline(m_handle);
        m_handle = nullptr;
    }
}
}