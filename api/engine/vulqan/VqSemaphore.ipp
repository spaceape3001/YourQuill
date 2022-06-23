////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VqSemaphore.hpp"
#include "VqStructs.hpp"
#include "VqException.hpp"
#include <basic/preamble.hpp>

namespace yq {
    namespace engine {
        VqSemaphore::VqSemaphore(VkDevice dev)
        {
            if(!dev)
                return ;
            m_device        = dev;
            VqSemaphoreCreateInfo   info;
            if(vkCreateSemaphore(m_device, &info, nullptr, &m_semaphore) != VK_SUCCESS)
                throw VqException("Unable to create semaphore!");
        }
        
        VqSemaphore::VqSemaphore(VqSemaphore&& mv)
        {
            move(std::move(mv));
        }
        
        VqSemaphore& VqSemaphore::operator=(VqSemaphore&&mv)
        {
            if(this != &mv){
                dtor();
                move(std::move(mv));
            }
            return *this;
        }
        
        VqSemaphore::~VqSemaphore()
        {
            dtor();
        }
        
        void    VqSemaphore::dtor()
        {
            if(m_semaphore){
                vkDestroySemaphore(m_device, m_semaphore, nullptr);
                m_semaphore = nullptr;
            }
        }
        
        void    VqSemaphore::move(VqSemaphore&& mv)
        {
            steal(m_device, mv.m_device);
            steal(m_semaphore, mv.m_semaphore);
        }
        
    }
}
