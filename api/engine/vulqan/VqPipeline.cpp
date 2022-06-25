////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqException.hpp"
#include "VqInternal.hpp"
#include "VqLogging.hpp"
#include "VqPipeline.hpp"
#include "VqShaderStages.hpp"
#include "VqStructs.hpp"

#include <basic/Logging.hpp>
#include <engine/Vulqan.hpp>
#include <engine/render/PipelineConfig.hpp>
#include <engine/render/StdPushConstant.hpp>
#include <math/Size2.hpp>
#include <math/Rectangle2.hpp>

namespace yq {
    namespace engine {
        VqPipeline::VqPipeline(VqInternal& win, const PipelineConfig& cfg)
        {
            m_device    = win.device;
            try {
                VqShaderStages stages(m_device, cfg.shaders);
                
                m_shaderMask    = stages.mask();

                VqPipelineVertexInputStateCreateInfo    vertexInfo;
                
                std::vector<VkVertexInputAttributeDescription>  attrs;
                std::vector<VkVertexInputBindingDescription>    vbos;
                
                for(uint32_t i=0;i<cfg.vbos.size();++i){
                    auto& v = cfg.vbos[i];
                    VkVertexInputBindingDescription b;
                    b.binding   = i;
                    b.stride    = v.stride;
                    b.inputRate = (VkVertexInputRate) v.inputRate.value();
                    vbos.push_back(b);
                    
                    for(auto& va : v.attrs){
                        VkVertexInputAttributeDescription   a;
                        a.binding       = i;
                        a.location      = va.location;
                        a.offset        = va.offset;
                        a.format        = (VkFormat) va.format.value();
                        attrs.push_back(a);
                    }
                }

                vertexInfo.vertexBindingDescriptionCount    = (uint32_t) vbos.size();
                vertexInfo.pVertexBindingDescriptions       = vbos.data();
                vertexInfo.vertexAttributeDescriptionCount  = (uint32_t) attrs.size();
                vertexInfo.pVertexAttributeDescriptions     = attrs.data();
                
                VqPipelineInputAssemblyStateCreateInfo  inputAssembly;
                inputAssembly.topology                  = (VkPrimitiveTopology) cfg.topology.value();
                inputAssembly.primitiveRestartEnable    = VK_FALSE;
                
                VkViewport viewport = win.dynamic.swapchain.def_viewport();

                VkRect2D scissor = win.dynamic.swapchain.def_scissor();
                
                VqPipelineViewportStateCreateInfo   viewportState{};
                viewportState.viewportCount = 1;
                viewportState.pViewports = &viewport;
                viewportState.scissorCount = 1;
                viewportState.pScissors = &scissor;
                
                VqPipelineRasterizationStateCreateInfo  rasterizer;
                rasterizer.depthClampEnable = VK_FALSE;
                rasterizer.rasterizerDiscardEnable = VK_FALSE;
                rasterizer.polygonMode = (VkPolygonMode) cfg.polymode.value();
                rasterizer.lineWidth = 1.0f;
                rasterizer.cullMode = (VkCullModeFlags) cfg.culling.value();
                rasterizer.frontFace = (VkFrontFace) cfg.front.value();
                rasterizer.depthBiasEnable = VK_FALSE;
                rasterizer.depthBiasConstantFactor = 0.0f; // Optional
                rasterizer.depthBiasClamp = 0.0f; // Optional
                rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

                VqPipelineMultisampleStateCreateInfo multisampling;
                multisampling.sampleShadingEnable = VK_FALSE;
                multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
                multisampling.minSampleShading = 1.0f; // Optional
                multisampling.pSampleMask = nullptr; // Optional
                multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
                multisampling.alphaToOneEnable = VK_FALSE; // Optional

                VkPipelineColorBlendAttachmentState colorBlendAttachment{};
                colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
                colorBlendAttachment.blendEnable = VK_FALSE;
                colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
                colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
                colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
                colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
                colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
                colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
                
                VqPipelineColorBlendStateCreateInfo colorBlending;
                colorBlending.logicOpEnable = VK_FALSE;
                colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
                colorBlending.attachmentCount = 1;
                colorBlending.pAttachments = &colorBlendAttachment;
                colorBlending.blendConstants[0] = 0.0f; // Optional
                colorBlending.blendConstants[1] = 0.0f; // Optional
                colorBlending.blendConstants[2] = 0.0f; // Optional
                colorBlending.blendConstants[3] = 0.0f; // Optional
                

                VkPushConstantRange push{};
                if(cfg.push.type != PushConfig::None){
                    push.offset     = 0;
                    switch(cfg.push.type){
                    case PushConfig::Full:
                    case PushConfig::View:
                        push.size   = sizeof(StdPushConstant);
                        break;
                    case PushConfig::Custom:
                        push.size   = cfg.push.size;
                        break;
                    default:
                        break;
                    }
                }
                
                VqPipelineLayoutCreateInfo pipelineLayoutInfo{};
                pipelineLayoutInfo.setLayoutCount = 0; // Optional
                pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
                if(push.size != 0){
                    if(cfg.push.shaders)
                        push.stageFlags = cfg.push.shaders;
                    else
                        push.stageFlags = m_shaderMask;
                    pipelineLayoutInfo.pushConstantRangeCount = 1;
                    pipelineLayoutInfo.pPushConstantRanges = &push;
                } else {
                    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
                    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
                }


                if (vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_layout) != VK_SUCCESS) 
                    throw VqException("Failed to create pipeline layout!");

                VqGraphicsPipelineCreateInfo pipelineInfo;
                pipelineInfo << stages;
                
                pipelineInfo.pVertexInputState = &vertexInfo;
                pipelineInfo.pInputAssemblyState = &inputAssembly;
                pipelineInfo.pViewportState = &viewportState;
                pipelineInfo.pRasterizationState = &rasterizer;
                pipelineInfo.pMultisampleState = &multisampling;
                pipelineInfo.pDepthStencilState = nullptr; // Optional
                pipelineInfo.pColorBlendState = &colorBlending;
                pipelineInfo.pDynamicState = nullptr; // Optional   
                pipelineInfo.layout = m_layout;
                pipelineInfo.renderPass = win.renderPass;
                pipelineInfo.subpass = 0;             
                pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
                pipelineInfo.basePipelineIndex = -1; // Optional        
                
                if(cfg.polymode == PolygonMode::Fill)
                    pipelineInfo.flags  = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT;
                if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS) 
                    throw VqException("Failed to create graphics pipeline!");
                
                    // if it's a fill polygon (typical), create a derivative wireframe pipeline
                if(cfg.polymode == PolygonMode::Fill){
                    pipelineInfo.flags  = VK_PIPELINE_CREATE_DERIVATIVE_BIT;
                    pipelineInfo.basePipelineHandle = m_pipeline;
                    pipelineInfo.basePipelineIndex  = -1;
                    rasterizer.polygonMode  = VK_POLYGON_MODE_LINE;
                    
                    if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_wireframe) != VK_SUCCESS)
                        throw VqException("Failed to create wireframe pipeline!");
                }
                return;
            }
            catch(VqException& ex)
            {
                vqError << ex.what();
            }
            dtor();
        }
        
        VqPipeline::VqPipeline(VqPipeline&& mv)
        {
            move(std::move(mv));
        }
        
        VqPipeline& VqPipeline::operator=(VqPipeline&& mv)
        {
            if(this != &mv){
                dtor();
                move(std::move(mv));
            }
            return *this;
        }
        
        VqPipeline::~VqPipeline()
        {
            dtor();
        }
        
        void    VqPipeline::dtor()
        {
            if(m_wireframe){
                vkDestroyPipeline(m_device, m_wireframe, nullptr);
                m_wireframe = nullptr;
            }
            if(m_pipeline){
                vkDestroyPipeline(m_device, m_pipeline, nullptr);
                m_pipeline  = nullptr;
            }
            if(m_layout){
                vkDestroyPipelineLayout(m_device, m_layout, nullptr);
                m_layout = nullptr;
            }
        }
        
        void    VqPipeline::move(VqPipeline&& mv)
        {
            steal(m_device, mv.m_device);
            steal(m_pipeline, mv.m_pipeline);
            steal(m_wireframe, mv.m_wireframe);
            steal(m_layout, mv.m_layout);
            m_shaderMask    = mv.m_shaderMask;
        }
    }
}
