////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqStructs.hpp"
#include "VqFencePool.hpp"
#include <engine/MainWindow.hpp>

namespace yq {
    VqFencePool::VqFencePool()
    {
    }
    
    VqFencePool::VqFencePool(MainWindow& v) : VqFencePool()
    {
        init(v);
    }
    
    VqFencePool::VqFencePool(VkDevice v) : VqFencePool()
    {
        init(v);
    }
    
    VqFencePool::~VqFencePool()
    {
        dtor();
    }

    VkFence     VqFencePool::allocate()
    {
        if(m_active < m_fences.size())
            return m_fences[m_active++];
        VkFence     f;
        VqFenceCreateInfo       fenceInfo;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        VkResult    r   = vkCreateFence(m_device, &fenceInfo, nullptr, &f);
        if(r != VK_SUCCESS)
            return nullptr;
        m_fences.push_back(f);
        ++m_active;
        return f;
    }
    
    void        VqFencePool::dtor()
    {
        wait_all();
        reset();
        for(auto& f : m_fences){
            vkDestroyFence(m_device, f, nullptr);
        }
        m_fences.clear();
    }

    VkFence     VqFencePool::fence(size_t i) const
    {
        return (i<m_active) ? m_fences[i] : nullptr;
    }

    bool        VqFencePool::init(MainWindow&v, bool reinit)
    {
        return init(v.logical(), reinit);
    }
    
    bool        VqFencePool::init(VkDevice v, bool reinit)
    {
        if(!v)
            return false;
            
        if(m_device){
            if(reinit){
                dtor();
            } else
                return false;
        }
    
        m_device    = v;
        return true;
    }

    VkResult    VqFencePool::reset(bool zero)
    {
        if(!m_device)
            return VK_ERROR_INITIALIZATION_FAILED;
        if(!m_active)
            return VK_SUCCESS;
        VkResult    res = vkResetFences(m_device, m_active, m_fences.data());
        if(res != VK_SUCCESS) [[unlikely]]
            return res;
        if(zero)
            m_active    = 0;
        return VK_SUCCESS;
    }

    VkResult    VqFencePool::wait(uint64_t timeout, bool all) const
    {
        if(!m_device)
            return VK_ERROR_INITIALIZATION_FAILED;
        if(!m_active)
            return VK_SUCCESS;
        return vkWaitForFences(m_device, m_active, m_fences.data(), all, timeout);
    }
    
    VkResult    VqFencePool::wait_all(uint64_t timeout) const
    {
        return wait(timeout, true);
    }
    
    VkResult    VqFencePool::wait_one(uint64_t timeout) const
    {
        return wait(timeout, false);
    }
}
