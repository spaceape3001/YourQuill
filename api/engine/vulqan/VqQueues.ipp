////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "VqQueues.hpp"

namespace yq {
    namespace engine {
        VqQueues::VqQueues(VkDevice dev, uint32_t family, uint32_t count) : m_family(family)
        {
            if(count > 0){
                m_queues.resize(count);
                for(uint32_t i=0;i<count;++i)
                    vkGetDeviceQueue(dev, family, i, &m_queues[i]);
            }
        }
        
        VqQueues::VqQueues(VqQueues&& mv)
        {
            move(std::move(mv));
        }
        
        VqQueues&          VqQueues::operator=(VqQueues&&mv)
        {   
            if(this != &mv){
                move(std::move(mv));
            }
            return *this;
        }
        
        VqQueues::~VqQueues()
        {
            m_family    = UINT32_MAX;
            m_queues.clear();
        }

        uint32_t            VqQueues::count() const
        {
            return m_queues.size();
        }
        
        VkQueue             VqQueues::operator[](uint32_t i) const
        {
            if(i<m_queues.size())
                return m_queues[i];
            return nullptr;
        }
        
        void    VqQueues::move(VqQueues&& mv)
        {
            m_family    = mv.m_family;
            m_queues    = std::move(mv.m_queues);
            mv.m_family = UINT32_MAX;
        }
    }
}
