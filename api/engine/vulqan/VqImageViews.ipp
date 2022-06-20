////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VqException.hpp"
#include "VqImageViews.hpp"
#include "VqStructs.hpp"
#include "VqSurface.hpp"
#include <basic/preamble.hpp>

namespace yq {
    VqImageViews::VqImageViews(VkDevice dev, const VqSurface&surf, const std::vector<VkImage>&images) : m_device(dev)
    {
        if(images.empty())
            return ;
            
        VqImageViewCreateInfo   createInfo;
        createInfo.viewType     = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format       = surf.format();
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        
        m_imageViews.resize(images.size());
        for(size_t i=0; i<images.size(); ++i){
            createInfo.image        = images[i];
            if(vkCreateImageView(m_device, &createInfo, nullptr, &m_imageViews[i]) != VK_SUCCESS) 
                throw VqException("Failed to create one of the Swap Image Viewers!");
        }
    }
    
    VqImageViews::VqImageViews(VqImageViews&&mv)
    {
        move(std::move(mv));
    }
    
    VqImageViews&          VqImageViews::operator=(VqImageViews&& mv)
    {
        if(this != &mv){
            dtor();
            move(std::move(mv));
        }
        return *this;
    }
    
    VqImageViews::~VqImageViews()
    {
        dtor();
    }
    
    VkImageView     VqImageViews::operator[](size_t i) const
    {
        return image_view(i);
    }
    
    VkImageView     VqImageViews::image_view(size_t i) const
    {
        if(i<m_imageViews.size())
            return m_imageViews[i];
        return nullptr;
    }
    
    
    void    VqImageViews::dtor()
    {
        for(auto iv : m_imageViews){
            if(iv)
                vkDestroyImageView(m_device, iv, nullptr);
        }
        m_imageViews.clear();
    }
    
    void    VqImageViews::move(VqImageViews&&mv)
    {
        steal(m_device, mv.m_device);
        m_imageViews = std::move(mv.m_imageViews);
    }
    
}
