// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include "pipeline_render.hpp"

#include "helpers_vulkan.hpp"

#include <logger.hpp>

namespace kirana::renderer
{
inline vk::PipelineVertexInputStateCreateInfo getVertexInput(const std::vector<VertexBufferLayout> &vb_layouts,
                                                             std::vector<vk::VertexInputBindingDescription> &
                                                             out_vertex_bindings,
                                                             std::vector<vk::VertexInputAttributeDescription> &
                                                             out_vertex_attribs)
{
    out_vertex_bindings.clear();
    out_vertex_attribs.clear();
    out_vertex_bindings.reserve(vb_layouts.size());
    for (uint32_t i = 0; i < vb_layouts.size(); ++i)
    {
        const auto &vb_layout = vb_layouts[i];
        out_vertex_bindings.emplace_back(i, vb_layout.stride,
                                         static_cast<vk::VertexInputRate>(static_cast<uint8_t>(vb_layout.input_rate)));
        for (uint32_t j = 0; j < vb_layout.attributes.size(); ++j)
        {
            const auto &vb_attrib = vb_layout.attributes[j];
            out_vertex_attribs.emplace_back(j, i, getFormat(vb_attrib.format), vb_attrib.offset);
        }
    }
    return vk::PipelineVertexInputStateCreateInfo{};
    return vk::PipelineVertexInputStateCreateInfo{
        vk::PipelineVertexInputStateCreateFlags{}, out_vertex_bindings, out_vertex_attribs};
}

inline vk::PipelineInputAssemblyStateCreateInfo getInputAssembly(const PrimitiveTopology topology)
{
    return vk::PipelineInputAssemblyStateCreateInfo{
        vk::PipelineInputAssemblyStateCreateFlags{},
        static_cast<vk::PrimitiveTopology>(static_cast<uint16_t>(topology)), false};
}

inline vk::PipelineViewportStateCreateInfo getViewportState(std::vector<vk::Viewport> &out_viewports,
                                                            std::vector<vk::Rect2D> &out_scissors)
{
    out_viewports = {vk::Viewport{0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f}};
    out_scissors = {vk::Rect2D{vk::Offset2D{0, 0}, vk::Extent2D{1280, 720}}};
    return vk::PipelineViewportStateCreateInfo{vk::PipelineViewportStateCreateFlags{}, out_viewports, out_scissors};
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

inline vk::PipelineColorBlendStateCreateInfo getColorBlendState(const std::vector<ColorAttachment> &attachments,
                                                                std::vector<vk::PipelineColorBlendAttachmentState> &
                                                                out_blend_attachments)
{
    const auto getBlendFactor = [](const BlendFactor factor) {
        return static_cast<vk::BlendFactor>(static_cast<uint8_t>(factor));
    };
    const auto getBlendOp = [](const BlendOperation op) {
        return static_cast<vk::BlendOp>(static_cast<uint8_t>(op));
    };
    const auto getColorMask = [](const ColorComponentFlags comp) {
        return static_cast<vk::ColorComponentFlags>(static_cast<uint8_t>(comp));
    };

    out_blend_attachments.clear();
    out_blend_attachments.reserve(attachments.size());
    for (const auto &[_, state] : attachments)
    {
        out_blend_attachments.emplace_back(state.enable_blend,
                                           getBlendFactor(state.src_color_blend_factor),
                                           getBlendFactor(state.dst_color_blend_factor),
                                           getBlendOp(state.color_blend_op),
                                           getBlendFactor(state.src_alpha_blend_factor),
                                           getBlendFactor(state.dst_alpha_blend_factor),
                                           getBlendOp(state.alpha_blend_op),
                                           getColorMask(state.color_write_mask)
            );
    }
    return vk::PipelineColorBlendStateCreateInfo{
        vk::PipelineColorBlendStateCreateFlags{}, false, vk::LogicOp::eCopy, out_blend_attachments};
}

inline vk::PipelineDynamicStateCreateInfo getDynamicState(const std::vector<vk::DynamicState> &dynamic_states)
{
    return vk::PipelineDynamicStateCreateInfo{vk::PipelineDynamicStateCreateFlags{}, dynamic_states};
}

inline vk::PipelineRenderingCreateInfo getRenderingState(const std::vector<ColorAttachment> &color_attachments,
                                                         const DepthStencilAttachment &depth_stencil_attachment,
                                                         std::vector<vk::Format> &out_color_formats,
                                                         vk::Format &out_depth_format)
{
    auto create_info = vk::PipelineRenderingCreateInfo{};
    out_color_formats.clear();
    out_color_formats.reserve(color_attachments.size());
    for (const auto &[format, _] : color_attachments)
    {
        out_color_formats.push_back(getFormat(format));
    }
    create_info.setColorAttachmentFormats(out_color_formats);
    if (depth_stencil_attachment.format != TextureFormat::UNKNOWN)
    {
        out_depth_format = getFormat(depth_stencil_attachment.format);
        create_info.setDepthAttachmentFormat(out_depth_format);
        create_info.setStencilAttachmentFormat(out_depth_format);
    }
    return create_info;
}

bool PipelineRender::init(const vk::Device device, const std::string &name, const PipelineLayout &layout,
                          const std::vector<Shader> &shaders, const RenderState &state)
{
    m_device = device;
    m_name = name;

    std::vector<vk::PipelineShaderStageCreateInfo> stage_create_infos{};
    stage_create_infos.reserve(shaders.size());
    for (const auto &s : shaders)
    {
        const auto num_stages = s.getStages().size();
        for (size_t i = 0; i < num_stages; ++i)
        {
            const auto stage = static_cast<vk::ShaderStageFlagBits>(static_cast<uint32_t>(s.getStages()[i]));
            const auto entry_point = s.getEntryPoints()[i].c_str();
            stage_create_infos.emplace_back(vk::PipelineShaderStageCreateFlags{}, stage, s.getNativeHandle(),
                                            entry_point);
        }
    }

    m_vertex_input_state = getVertexInput(state.vertex_buffer_layouts, m_vertex_input_bindings,
                                          m_vertex_input_attributes);
    m_input_assembly_state = getInputAssembly(state.rasterization.topology);
    m_viewport_state = getViewportState(m_viewports, m_scissor_rects);
    m_rasterization_state = getRasterState(state.rasterization);
    m_multisample_state = getMultisampleState(state.multisample);
    m_depth_stencil_state = getDepthStencilState(state.depth_stencil_attachment.state);
    m_color_blend_state = getColorBlendState(state.color_attachments, m_color_blend_attachments);
    m_dynamic_state = getDynamicState(m_dynamic_states);

    auto create_info = vk::GraphicsPipelineCreateInfo{vk::PipelineCreateFlags{}, stage_create_infos,
                                                      &m_vertex_input_state, &m_input_assembly_state, {},
                                                      &m_viewport_state, &m_rasterization_state,
                                                      &m_multisample_state,
                                                      &m_depth_stencil_state, &m_color_blend_state,
                                                      &m_dynamic_state, layout.getNativeHandle()};

    m_rendering_state = getRenderingState(state.color_attachments, state.depth_stencil_attachment, m_color_formats,
                                          m_depth_format);
    create_info.pNext = &m_rendering_state;

    const auto result = m_device.createGraphicsPipeline(nullptr, create_info);
    if (result.result != vk::Result::eSuccess)
    {
        core::Logger::error(LOG_CHANNEL_VULKAN, "Failed to create graphics pipeline: " + vk::to_string(result.result));
        return false;
    }
    m_handle = result.value;

    if (!m_name.empty())
    {
        const auto handle = reinterpret_cast<uint64_t>(static_cast<VkPipeline>(m_handle));
        setDebugName(m_device, vk::ObjectType::ePipeline, handle, m_name);
    }
    return true;
}

void PipelineRender::destroy()
{
    if (m_device && m_handle)
    {
        m_device.destroyPipeline(m_handle);
        m_handle = nullptr;
    }
}
}