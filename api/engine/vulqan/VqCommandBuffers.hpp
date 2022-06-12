////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <vulkan/vulkan_core.h>
#include <vector>

namespace yq {
    namespace engine {
        class VqCommandPool;
    
        class VqCommandBuffers : trait::not_copyable {
        public:
            VqCommandBuffers(){}
            explicit VqCommandBuffers(const VqCommandPool&, uint32_t count=1, VkCommandBufferLevel level=VK_COMMAND_BUFFER_LEVEL_PRIMARY);
            VqCommandBuffers(VqCommandBuffers&&);
            VqCommandBuffers& operator=(VqCommandBuffers&&);
            ~VqCommandBuffers();
            
            bool    good() const { return !empty(); }
            operator bool() const { return !empty(); }

            VkCommandBuffer operator[](size_t) const;
            size_t                  count() const { return m_buffers.size(); }
            bool                    empty() const { return m_buffers.empty(); }
            size_t                  size() const { return m_buffers.size(); }
            const VkCommandBuffer*  data() const { return m_buffers.data(); }
            VkCommandBuffer*        data() { return m_buffers.data(); }

        private:
            void    dtor();
            void    move(VqCommandBuffers&&);
            
            VkDevice                        m_device    = nullptr;
            VkCommandPool                   m_pool      = nullptr;
            std::vector<VkCommandBuffer>    m_buffers;
        };
    }
}
