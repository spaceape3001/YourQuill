////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <vulkan/vulkan_core.h>
#include <vk_mem_alloc.h>

namespace yq {
    namespace engine {
        class VqVisualizer;

        class VqAllocator : trait::not_copyable {
        public:
            VqAllocator(){}
            explicit VqAllocator(VqVisualizer&, size_t chunkSize=0);
            VqAllocator(VqAllocator&&);
            VqAllocator& operator=(VqAllocator&&);
            ~VqAllocator();
            
            bool    good() const noexcept { return m_allocator != nullptr; }
            operator bool() const noexcept { return good(); }
            operator VmaAllocator() const noexcept { return m_allocator; }
            VmaAllocator allocator() const noexcept { return m_allocator; }
        
        private:
            void    dtor();
            void    move(VqAllocator&&);
            
            VmaAllocator        m_allocator      = nullptr;
        };
    }
}
