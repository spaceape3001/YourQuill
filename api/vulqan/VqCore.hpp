////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Result.hpp>
#include <vulkan/vulkan_core.h>
#include <vector>
#include <set>
#include <string>

namespace yq {
    struct VqApplicationInfo : public VkApplicationInfo {
        VqApplicationInfo() : VkApplicationInfo{}
        {
            sType   = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        }
    };

    struct VqDeviceCreateInfo : public VkDeviceCreateInfo {
        VqDeviceCreateInfo() : VkDeviceCreateInfo{}
        {
            sType   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        }
    };

    struct VqDeviceQueueCreateInfo : public VkDeviceQueueCreateInfo {
        VqDeviceQueueCreateInfo() : VkDeviceQueueCreateInfo{}
        {
            sType   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        }
    };

    struct VqInstanceCreateInfo : public VkInstanceCreateInfo {
        VqInstanceCreateInfo() : VkInstanceCreateInfo{}
        {
            sType   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        }
    };



    std::vector<VkExtensionProperties>      vqExtensionInstanceExtensionProperties(const char* z=nullptr);
    std::vector<VkLayerProperties>          vqEnumerateInstanceLayerProperties();

    std::vector<VkQueueFamilyProperties>    vqGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice);
    std::vector<const char*>                vqGlfwRequiredExtensions();
    std::vector<VkPhysicalDevice>           vqEnumeratePhysicalDevices(VkInstance);

    std::set<std::string>                   vqNameSet(const std::vector<VkExtensionProperties>&);
    std::set<std::string>                   vqNameSet(const std::vector<VkLayerProperties>&);
    VkQueueFlags                            vqOrFlags(const std::vector<VkQueueFamilyProperties>&);

    Result<uint32_t>                        vqFindFirstGraphicsQueue(const std::vector<VkQueueFamilyProperties>&);
    Result<uint32_t>                        vqFindFirstPresentQueue(VkPhysicalDevice, VkSurfaceKHR);
}

