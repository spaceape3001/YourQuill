////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vulkan/vulkan_core.h>
#include <string_view>

namespace yq {
    class VqGPU {
    public:
    
        VqGPU() {}
        explicit VqGPU(VkPhysicalDevice);
        VqGPU(const VqGPU&);
        VqGPU& operator=(const VqGPU&);
        ~VqGPU();
        
        operator VkPhysicalDevice() const noexcept { return m_physical; }
        
        VkPhysicalDeviceType    device_type() const { return m_info.deviceType; }
        std::string_view        device_name() const;
        
        uint32_t        max_memory_allocation_count() const noexcept { return m_info.limits.maxMemoryAllocationCount; }
        uint32_t        max_push_constants_size() const noexcept { return m_info.limits.maxPushConstantsSize; }
        uint32_t        max_viewports() const noexcept { return m_info.limits.maxViewports; }
        
        
    private:
        VkPhysicalDeviceProperties          m_info;
        VkPhysicalDeviceMemoryProperties    m_memory;
        VkPhysicalDevice                    m_physical  = nullptr;
    };

}

