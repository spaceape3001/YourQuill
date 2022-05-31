////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqFence.hpp"
#include "VqStructs.hpp"
#include "VqWindow.hpp"
#include <basic/Logging.hpp>

namespace yq {
    VqFence::VqFence()
    {
    }
    
    VqFence::VqFence(VqWindow& v) : VqFence(v.logical())
    {
    }
    
    VqFence::VqFence(VkDevice v)
    {
        m_device    = v;
        if(v){
            VqFenceCreateInfo   fci;
            fci.flags = VK_FENCE_CREATE_SIGNALED_BIT;
            VkResult    r   = vkCreateFence(m_device, &fci, nullptr,  &m_fence);
            if(r != VK_SUCCESS){
                yError() << "Unable to create vulkan fence!";
                m_device    = nullptr;
                m_fence     = nullptr;
            }
        }
    }

    void        VqFence::dtor()
    {
        if(m_fence && m_device){
            vkDestroyFence(m_device, m_fence, nullptr);
            m_fence     = nullptr;
            m_device    = nullptr;
        }
    }
    
    VqFence::VqFence(VqFence&&mv)
    {
        std::swap(m_fence, mv.m_fence);
        std::swap(m_device, mv.m_device);
    }
    
    VqFence&    VqFence::operator=(VqFence&& mv)
    {
        if(this != &mv){
            dtor();
            std::swap(m_fence, mv.m_fence);
            std::swap(m_device, mv.m_device);
        }
        return *this;
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
