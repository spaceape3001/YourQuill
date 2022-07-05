////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VqException.hpp"
#include "VqFrameBuffers.hpp"
#include "VqStructs.hpp"
#include <basic/preamble.hpp>

namespace yq {
    namespace engine {
        VqFrameBuffers::VqFrameBuffers(VkDevice dev, VkRenderPass render, const VkExtent2D&extents, const std::vector<VkImageView>&imageviews) : m_device(dev)
        {
            if(imageviews.empty())
                return ;
                
            VqFramebufferCreateInfo   createInfo;
            createInfo.renderPass       = render;
            createInfo.attachmentCount  = 1;
            createInfo.width            = extents.width;
            createInfo.height           = extents.height;
            createInfo.layers           = 1;
            
            m_framebuffers.resize(imageviews.size(), nullptr);
            for(size_t i=0;i<imageviews.size();++i){
                createInfo.pAttachments = &imageviews[i];
                if (vkCreateFramebuffer(m_device, &createInfo, nullptr, &m_framebuffers[i]) != VK_SUCCESS) 
                    throw VqException("Failed to create framebuffer!");
            }
        }
        
        VqFrameBuffers::VqFrameBuffers(VqFrameBuffers&&mv)
        {
            move(std::move(mv));
        }
        
        VqFrameBuffers&          VqFrameBuffers::operator=(VqFrameBuffers&& mv)
        {
            if(this != &mv){
                dtor();
                move(std::move(mv));
            }
            return *this;
        }
        
        VqFrameBuffers::~VqFrameBuffers()
        {
            dtor();
        }
        
        VkFramebuffer     VqFrameBuffers::operator[](size_t i) const
        {
            return framebuffer(i);
        }
        
        VkFramebuffer     VqFrameBuffers::framebuffer(size_t i) const
        {
            if(i<m_framebuffers.size())
                return m_framebuffers[i];
            return nullptr;
        }
        
        
        void    VqFrameBuffers::dtor()
        {
            for(auto iv : m_framebuffers){
                if(iv)
                    vkDestroyFramebuffer(m_device, iv, nullptr);
            }
            m_framebuffers.clear();
        }
        
        void    VqFrameBuffers::move(VqFrameBuffers&&mv)
        {
            steal(m_device, mv.m_device);
            m_framebuffers = std::move(mv.m_framebuffers);
        }
        
    }
}
