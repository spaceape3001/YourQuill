////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vulkan/vulkan_core.h>
#include <basic/trait/not_copyable.hpp>

namespace yq {
    namespace engine {
        class VqFence : trait::not_copyable {
        public:
        
            VqFence(){}
            VqFence(VkDevice);
            
            VqFence(VqFence&&);
            VqFence&    operator=(VqFence&&);
            
            VkResult    wait(uint64_t timeout=UINT64_MAX) const;
            VkResult    reset();
            
            VkFence     fence() const { return m_fence; }
            
            operator    VkFence() const noexcept { return m_fence; }
            
            VkResult    wait_reset(uint64_t timeout=UINT64_MAX);
            
            bool        good() const { return m_fence != nullptr; }
        
        private:
            void    dtor();
            void    move(VqFence&&);
            VkDevice        m_device = nullptr;
            VkFence         m_fence  = nullptr;
        };
    }
}
