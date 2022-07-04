////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VqAllocator.hpp"
#include "VqVisualizer.hpp"
#include <basic/preamble.hpp>
#include <engine/Application.hpp>

namespace yq {
    namespace engine {
        VqAllocator::VqAllocator(VqVisualizer& win, size_t chunkSize)
        {
            VmaAllocatorCreateInfo  createInfo{};
            createInfo.instance         = Application::vulkan();
            createInfo.physicalDevice   = win.physical;
            createInfo.device           = win.device;
            vmaCreateAllocator(&createInfo, &m_allocator);
        }
        
        VqAllocator::VqAllocator(VqAllocator&& mv)
        {
            move(std::move(mv));
        }
        
        VqAllocator& VqAllocator::operator=(VqAllocator&&mv)
        {
            if(this != &mv){
                dtor();
                move(std::move(mv));
            }
            return *this;
        }
        
        VqAllocator::~VqAllocator()
        {
            dtor();
        }
        
        void    VqAllocator::dtor()
        {
            if(m_allocator){
                vmaDestroyAllocator(m_allocator);
                m_allocator = nullptr;
            }
        }
        
        void    VqAllocator::move(VqAllocator&& mv)
        {
            steal(m_allocator, mv.m_allocator);
        }
        
    }
}
