////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqCommandPool.hpp"
#include "VqException.hpp"
#include "VqStructs.hpp"

namespace yq {
    namespace engine {
        VqCommandPool::VqCommandPool(VkDevice dev, uint32_t queueFamily, VkCommandPoolCreateFlags flags) : m_device(dev)
        {
            VqCommandPoolCreateInfo poolInfo;
            poolInfo.flags              = flags;
            poolInfo.queueFamilyIndex   = queueFamily;
            if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_pool) != VK_SUCCESS) 
                throw VqException("Failed to create command pool!");
        }
        
        VqCommandPool::VqCommandPool(VqCommandPool&&mv)
        {
            move(std::move(mv));
        }
        
        VqCommandPool&          VqCommandPool::operator=(VqCommandPool&&mv)
        {
            if(this != &mv){
                dtor();
                move(std::move(mv));
            }
            return *this;
        }
        
        VqCommandPool::~VqCommandPool()
        {
            dtor();
        }
        
        void    VqCommandPool::dtor()
        {
            if(m_pool){
                vkDestroyCommandPool(m_device, m_pool, nullptr);
                m_pool  = nullptr;
            }
        }
        
        void    VqCommandPool::move(VqCommandPool&& mv)
        {
            m_device    = mv.m_device;
            m_pool      = mv.m_pool;
            mv.m_pool   = nullptr;
        }
        
    }
}
