////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Result.hpp>
#include <vulkan/vulkan_core.h>

#include <optional>
#include <set>
#include <string>
#include <vector>

/*
    The VqCore is about providing simple wrappers for boiler plate code, using the prefix "Vq" in lieu of "Vk".  
    For structures, it's adding in the proper stype assignment. 
*/


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
    
    struct VqImageViewCreateInfo : public VkImageViewCreateInfo {
        VqImageViewCreateInfo() : VkImageViewCreateInfo{}
        {
            sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        }
    };

    struct VqInstanceCreateInfo : public VkInstanceCreateInfo {
        VqInstanceCreateInfo() : VkInstanceCreateInfo{}
        {
            sType   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        }
    };
    
    struct VqPipelineShaderStageCreateInfo : public VkPipelineShaderStageCreateInfo {
        VqPipelineShaderStageCreateInfo() : VkPipelineShaderStageCreateInfo{}
        {
            sType   = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        }
    };
    
    struct VqShaderModuleCreateInfo : public VkShaderModuleCreateInfo {
        VqShaderModuleCreateInfo() : VkShaderModuleCreateInfo{}
        {
            sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        }
    };
    
    struct VqSwapchainCreateInfoKHR  : public VkSwapchainCreateInfoKHR {
        VqSwapchainCreateInfoKHR() : VkSwapchainCreateInfoKHR{}
        {
            sType   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        }
    };
    
    #ifdef WIN32
    struct VqWin32SurfaceCreateInfoKHR : public VkWin32SurfaceCreateInfoKHR {
        VqWin32SurfaceCreateInfoKHR() : VkWin32SurfaceCreateInfoKHR{}
        {
            sType   = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        }
    };
    #endif

    std::vector<VkExtensionProperties>              vqEnumerateDeviceExtensionProperties(VkPhysicalDevice, const char* layerName=nullptr);
    std::vector<VkLayerProperties>                  vqEnumerateDeviceLayerProperties(VkPhysicalDevice);
    std::vector<VkExtensionProperties>              vqEnumerateInstanceExtensionProperties(const char* layerName=nullptr);
    std::vector<VkLayerProperties>                  vqEnumerateInstanceLayerProperties();
    std::vector<VkPhysicalDeviceGroupProperties>    vqEnumeratePhysicalDeviceGroups(VkInstance i=nullptr);
    std::vector<VkPhysicalDevice>                   vqEnumeratePhysicalDevices(VkInstance i=nullptr);
    
    
    VkPhysicalDevice                        vqFirstDevice(VkInstance i=nullptr);
    
    // not in the SDK
    //std::vector<VkPhysicalDeviceGroupProperties>    vqEnumeratePhysicalDeviceGroupsKHR(VkInstance);

    std::vector<VkQueueFamilyProperties>    vqGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice);
    std::vector<VkSurfaceFormatKHR>         vqGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice, VkSurfaceKHR);
    std::vector<VkPresentModeKHR>           vqGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice, VkSurfaceKHR);
    std::vector<const char*>                vqGlfwRequiredExtensions();

    std::string                             vqName(VkPhysicalDevice);

    std::set<std::string>                   vqNameSet(const std::vector<VkExtensionProperties>&);
    std::set<std::string>                   vqNameSet(const std::vector<VkLayerProperties>&);
    VkQueueFlags                            vqOrFlags(const std::vector<VkQueueFamilyProperties>&);

    Result<uint32_t>                        vqFindFirstGraphicsQueue(const std::vector<VkQueueFamilyProperties>&);
    Result<uint32_t>                        vqFindFirstPresentQueue(VkPhysicalDevice, VkSurfaceKHR);
    
    

    struct VqQueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
    };

    VqQueueFamilyIndices                    vqFindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR);


    std::string_view                        to_string(VkColorSpaceKHR);
    std::string_view                        to_string(VkFormat);
}

