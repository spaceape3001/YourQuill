////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqGPU.hpp"
#include <basic/TextUtils.hpp>

namespace yq {
    namespace engine {
        VqGPU::VqGPU(const VqGPU&) = default;
        VqGPU::~VqGPU() = default;
        VqGPU& VqGPU::operator=(const VqGPU&) = default;

        VqGPU::VqGPU(VkPhysicalDevice dev)
        {
            m_physical  = dev;
            if(m_physical){
                vkGetPhysicalDeviceProperties(m_physical, &m_info);
                vkGetPhysicalDeviceMemoryProperties(m_physical, &m_memory);
            }
        }

        std::string_view        VqGPU::device_name() const
        {
            return std::string_view(m_info.deviceName, strnlen(m_info.deviceName, VK_MAX_PHYSICAL_DEVICE_NAME_SIZE));
        }

    }
}
