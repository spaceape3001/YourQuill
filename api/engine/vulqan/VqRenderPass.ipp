////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VqRenderPass.hpp"
#include "VqStructs.hpp"
#include "VqInternal.hpp"
#include "VqException.hpp"
#include <basic/preamble.hpp>
#include <engine/MainWindow.hpp>

namespace yq {

    VqRenderPass::VqRenderPass(VqInternal& win)
    {
        m_device        = win.device;

            //  Render pass
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = win.surface.format();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;        
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;        

        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        
        VqRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        if (vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_render) != VK_SUCCESS) 
            throw VqException("Unable to create the render pass!");
    }
    
    VqRenderPass::VqRenderPass(VqRenderPass&& mv)
    {
        move(std::move(mv));
    }
    
    VqRenderPass& VqRenderPass::operator=(VqRenderPass&&mv)
    {
        if(this != &mv){
            dtor();
            move(std::move(mv));
        }
        return *this;
    }
    
    VqRenderPass::~VqRenderPass()
    {
        dtor();
    }
    
    void    VqRenderPass::dtor()
    {
        if(m_render){
            vkDestroyRenderPass(m_device, m_render, nullptr);
            m_render = nullptr;
        }
    }
    
    void    VqRenderPass::move(VqRenderPass&& mv)
    {
        steal(m_device, mv.m_device);
        steal(m_render, mv.m_render);
    }
}
