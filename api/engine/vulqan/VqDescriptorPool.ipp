////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VqDescriptorPool.hpp"
#include "VqStructs.hpp"
#include "VqException.hpp"
#include <basic/preamble.hpp>
#include <algorithm>

namespace yq {
    namespace engine {

        VqDescriptorPool::VqDescriptorPool(VkDevice dev, uint32_t cnt)
        {
            if(!dev)
                return ;

            m_device        = dev;
            m_count         = std::max(kMinCount, cnt);

            std::vector<VkDescriptorPoolSize> pool_sizes =
            {
                { VK_DESCRIPTOR_TYPE_SAMPLER, m_count },
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, m_count },
                { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, m_count },
                { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, m_count },
                { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, m_count },
                { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, m_count },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, m_count },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, m_count },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, m_count },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, m_count },
                { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, m_count }
            };
            VqDescriptorPoolCreateInfo pool_info;
            pool_info.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            pool_info.maxSets       = m_count * pool_sizes.size();
            pool_info.poolSizeCount = (uint32_t) pool_sizes.size();
            pool_info.pPoolSizes    = pool_sizes.data();
            if(vkCreateDescriptorPool(m_device, &pool_info, nullptr, &m_pool) != VK_SUCCESS)
                throw VqException("Unable to allocate the descriptor pool!");
        }
        
        VqDescriptorPool::VqDescriptorPool(VqDescriptorPool&& mv)
        {
            move(std::move(mv));
        }
        
        VqDescriptorPool& VqDescriptorPool::operator=(VqDescriptorPool&&mv)
        {
            if(this != &mv){
                dtor();
                move(std::move(mv));
            }
            return *this;
        }
        
        VqDescriptorPool::~VqDescriptorPool()
        {
            dtor();
        }
        
        void    VqDescriptorPool::dtor()
        {
            if(m_pool){
                vkDestroyDescriptorPool(m_device, m_pool, nullptr);
                m_pool = nullptr;
            }
            
            m_count     = 0;
            m_device    = nullptr;
        }
        
        void    VqDescriptorPool::move(VqDescriptorPool&& mv)
        {
            steal(m_device, mv.m_device);
            steal(m_pool, mv.m_pool);
            m_count     = mv.m_count;
        }
        
    }
}
