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
        class VqDescriptorPool : trait::not_copyable {
        public:
            static constexpr const uint32_t kMinCount = 1000;

            VqDescriptorPool(){}
            explicit VqDescriptorPool(VkDevice, uint32_t cnt=0);
            VqDescriptorPool(VqDescriptorPool&&);
            VqDescriptorPool& operator=(VqDescriptorPool&&);
            ~VqDescriptorPool();
            
            bool    good() const noexcept { return m_pool != nullptr; }
            operator bool() const noexcept { return good(); }
            operator VkDescriptorPool() const noexcept { return m_pool; }
            VkDescriptorPool descriptor_pool() const noexcept { return m_pool; }
            uint32_t    count() const { return m_count; }
        
        private:
            void    dtor();
            void    move(VqDescriptorPool&&);
            
            VkDevice            m_device    = nullptr;
            VkDescriptorPool    m_pool      = nullptr;
            uint32_t            m_count     = 0;
        };
    }
}
