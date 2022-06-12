////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqCommandBuffers.hpp"
#include "VqCommandPool.hpp"
#include "VqException.hpp"
#include "VqStructs.hpp"
#include <basic/preamble.hpp>

namespace yq {
    namespace engine {

        VqCommandBuffers::VqCommandBuffers(const VqCommandPool&pool, uint32_t count, VkCommandBufferLevel level)
        {
            m_device    = pool.device();
            m_pool      = pool.pool();
            if(!count)
                return;

            VqCommandBufferAllocateInfo allocInfo;
            allocInfo.commandPool = m_pool;
            allocInfo.level = level;
            allocInfo.commandBufferCount = count;
            m_buffers.resize(count, nullptr);
            if (vkAllocateCommandBuffers(m_device, &allocInfo, m_buffers.data()) != VK_SUCCESS) 
                throw VqException("Failed to allocate command buffers!");
        }
        
        VqCommandBuffers::VqCommandBuffers(VqCommandBuffers&&mv)
        {
            move(std::move(mv));
        }
        
        VqCommandBuffers& VqCommandBuffers::operator=(VqCommandBuffers&&mv)
        {
            if(this != &mv){
                dtor();
                move(std::move(mv));
            }
            return *this;
        }
        
        VqCommandBuffers::~VqCommandBuffers()
        {
            dtor();
        }
        
        VkCommandBuffer VqCommandBuffers::operator[](size_t i) const
        {
            if(i<m_buffers.size())
                return m_buffers[i];
            return nullptr;
        }
        
        void    VqCommandBuffers::dtor()
        {
            if(!m_buffers.empty()){
                vkFreeCommandBuffers(m_device, m_pool, (uint32_t) m_buffers.size(), m_buffers.data());
                m_buffers.clear();
            }
        }
        
        void    VqCommandBuffers::move(VqCommandBuffers&&mv)
        {
            steal(m_device, mv.m_device);
            steal(m_pool, mv.m_pool);
            m_buffers   = std::move(mv.m_buffers);
            mv.m_buffers.clear();
        }
    }
}
