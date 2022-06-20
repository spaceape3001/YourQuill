////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VqException.hpp"
#include "VqFence.hpp"
#include "VqStructs.hpp"

namespace yq {
    VqFence::VqFence(VkDevice dev)
    {
        if(!dev)
            return ;
        m_device    = dev;
        VqFenceCreateInfo   fci;
        fci.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        if(vkCreateFence(m_device, &fci, nullptr,  &m_fence) != VK_SUCCESS)
            throw VqException("Unable to create fence!");
    }

    void        VqFence::dtor()
    {
        if(m_fence){
            vkDestroyFence(m_device, m_fence, nullptr);
            m_fence     = nullptr;
        }
        m_device    = nullptr;
    }
    
    VqFence::VqFence(VqFence&&mv)
    {
            move(std::move(mv));
    }
    
    VqFence&    VqFence::operator=(VqFence&& mv)
    {
        if(this != &mv){
            dtor();
            move(std::move(mv));
        }
        return *this;
    }
    
    void    VqFence::move(VqFence&&mv)
    {
        steal(m_fence, mv.m_fence);
        steal(m_device, mv.m_device);
    }

    VkResult    VqFence::reset()
    {
        if(!m_fence) [[unlikely]]
            return VK_ERROR_INITIALIZATION_FAILED;
        return vkResetFences(m_device, 1, &m_fence);
    }
    
    VkResult    VqFence::wait(uint64_t timeout) const
    {
        if(!m_fence) [[unlikely]]
            return VK_ERROR_INITIALIZATION_FAILED;
        return vkWaitForFences(m_device, 1, &m_fence, VK_TRUE, timeout);
    }

    VkResult    VqFence::wait_reset(uint64_t timeout)
    {
        VkResult    r   = wait(timeout);
        if(r != VK_SUCCESS)
            return r;
        return reset();
    }

}
