////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqException.hpp"
#include "VqFramebuffers.hpp"
#include "VqRenderPass.hpp"
#include "VqStructs.hpp"
#include "VqSurface.hpp"
#include <basic/preamble.hpp>

namespace yq {
    namespace engine {
        VqFramebuffers::VqFramebuffers(VkDevice dev, VqRenderPass&render, const VkExtent2D&extents, const std::vector<VkImageView>&imageviews) : m_device(dev)
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
        
        VqFramebuffers::VqFramebuffers(VqFramebuffers&&mv)
        {
            move(std::move(mv));
        }
        
        VqFramebuffers&          VqFramebuffers::operator=(VqFramebuffers&& mv)
        {
            if(this != &mv){
                dtor();
                move(std::move(mv));
            }
            return *this;
        }
        
        VqFramebuffers::~VqFramebuffers()
        {
            dtor();
        }
        
        VkFramebuffer     VqFramebuffers::operator[](size_t i) const
        {
            return framebuffer(i);
        }
        
        VkFramebuffer     VqFramebuffers::framebuffer(size_t i) const
        {
            if(i<m_framebuffers.size())
                return m_framebuffers[i];
            return nullptr;
        }
        
        
        void    VqFramebuffers::dtor()
        {
            for(auto iv : m_framebuffers){
                if(iv)
                    vkDestroyFramebuffer(m_device, iv, nullptr);
            }
            m_framebuffers.clear();
        }
        
        void    VqFramebuffers::move(VqFramebuffers&&mv)
        {
            steal(m_device, mv.m_device);
            m_framebuffers = std::move(mv.m_framebuffers);
        }
        
    }
}
