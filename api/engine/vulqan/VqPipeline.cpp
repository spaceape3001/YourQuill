////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqCore.hpp"
#include "VqPipeline.hpp"
#include "VqWindow.hpp"

#include <basic/Logging.hpp>
#include <basic/Safety.hpp>
#include <engine/shader/Shader.hpp>

namespace yq {
    VqPipeline::VqPipeline(VqWindow& win, const PipelineConfig& config)
    {
        VqShaderStages      stages(win, config.shaders);
        
        
        VqPipelineVertexInputStateCreateInfo    vertexInfo;
        vertexInfo.vertexBindingDescriptionCount    = 0;
        vertexInfo.pVertexBindingDescriptions       = nullptr;
        vertexInfo.vertexAttributeDescriptionCount  = 0;
        vertexInfo.pVertexAttributeDescriptions     = nullptr;
        
        VqPipelineInputAssemblyStateCreateInfo  inputAssembly;
        inputAssembly.topology                  = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable    = VK_FALSE;
        
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) win.m_swapExtent.width;
        viewport.height = (float) win.m_swapExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = win.m_swapExtent;        
        
        VqPipelineViewportStateCreateInfo   viewportState{};
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;
        
        VqPipelineRasterizationStateCreateInfo  rasterizer;
        
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; // Optional
        rasterizer.depthBiasClamp = 0.0f; // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

    }

    VqPipeline::~VqPipeline()
    {
    }
}
