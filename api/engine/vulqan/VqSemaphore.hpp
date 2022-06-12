////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <vulkan/vulkan_core.h>

namespace yq {
    namespace engine {
        class VqSemaphore : trait::not_copyable {
        public:
            VqSemaphore(){}
            VqSemaphore(VkDevice);
            VqSemaphore(VqSemaphore&&);
            VqSemaphore& operator=(VqSemaphore&&);
            ~VqSemaphore();
            
            bool    good() const noexcept { return m_semaphore != nullptr; }
            operator bool() const noexcept { return good(); }
            operator VkSemaphore() const noexcept { return m_semaphore; }
            VkSemaphore semaphore() const noexcept { return m_semaphore; }
        
        private:
            void    dtor();
            void    move(VqSemaphore&&);
            
            VkDevice    m_device    = nullptr;
            VkSemaphore m_semaphore = nullptr;
        };
    }
}
