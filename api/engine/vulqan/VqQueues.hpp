////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <vulkan/vulkan_core.h>
#include <vector>

namespace yq {
    class VqQueues : trait::not_copyable {
    public:
        VqQueues() {}
        VqQueues(VkDevice, uint32_t family, uint32_t count=1);
        VqQueues(VqQueues&&);
        VqQueues&          operator=(VqQueues&&);
        ~VqQueues();
        
        VkQueue             operator[](uint32_t) const;
        uint32_t            family() const { return m_family; }
        uint32_t            count() const;
        
    private:
        
        void    move(VqQueues&&);

        std::vector<VkQueue>    m_queues;
        uint32_t                m_family = UINT32_MAX;
    };

}
