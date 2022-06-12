////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <vulkan/vulkan_core.h>

struct GLFWwindow;


namespace yq {
    namespace engine {
        class VqCommandPool : trait::not_copyable {
        public:
            VqCommandPool() {}
            VqCommandPool(VkDevice, uint32_t queueFamily, VkCommandPoolCreateFlags flags=VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
            VqCommandPool(VqCommandPool&&);
            VqCommandPool&          operator=(VqCommandPool&&);
            ~VqCommandPool();
            
            VkCommandPool       pool() const { return m_pool; }
            operator bool () const { return m_pool != nullptr; }
            operator VkCommandPool() const { return m_pool; }
            VkDevice            device() const { return m_device; }
            
        private:
            void    dtor();
            void    move(VqCommandPool&&);
            VkDevice        m_device    = nullptr;
            VkCommandPool   m_pool      = nullptr;
        };
    
    }
}
