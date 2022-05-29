////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqGLFW.hpp"
#include "VqCore.hpp"
#include <basic/CollectionUtils.hpp>

namespace yq {
    std::vector<VkExtensionProperties>   vqEnumerateDeviceExtensionProperties(VkPhysicalDevice device, const char* layerName)
    {
        uint32_t                        count  = 0;
        vkEnumerateDeviceExtensionProperties(device, layerName, &count, nullptr);
        std::vector<VkExtensionProperties>   ret(count);
        if(count)
            vkEnumerateDeviceExtensionProperties(device, layerName, &count, ret.data());
        return ret;
    }

    std::vector<VkLayerProperties>       vqEnumerateDeviceLayerProperties(VkPhysicalDevice device)
    {
        uint32_t                        count  = 0;
        vkEnumerateDeviceLayerProperties(device, &count, nullptr);
        std::vector<VkLayerProperties>   ret(count);
        if(count)
            vkEnumerateDeviceLayerProperties(device, &count, ret.data());
        return ret;
    }

    std::vector<VkExtensionProperties>   vqEnumerateInstanceExtensionProperties(const char* layerName)
    {
        uint32_t                        count  = 0;
        vkEnumerateInstanceExtensionProperties(layerName, &count, nullptr);
        std::vector<VkExtensionProperties>   ret(count);
        if(count)
            vkEnumerateInstanceExtensionProperties(layerName, &count, ret.data());
        return ret;
    }

    std::vector<VkLayerProperties>       vqEnumerateInstanceLayerProperties()
    {
        uint32_t                        count  = 0;
        vkEnumerateInstanceLayerProperties(&count, nullptr);
        std::vector<VkLayerProperties>   ret(count);
        if(count)
            vkEnumerateInstanceLayerProperties(&count, ret.data());
        return ret;
    }

    std::vector<VkPhysicalDeviceGroupProperties>    vqEnumeratePhysicalDeviceGroups(VkInstance inst)
    {
        uint32_t    count   = 0;
        vkEnumeratePhysicalDeviceGroups(inst, &count, nullptr);
        std::vector<VkPhysicalDeviceGroupProperties>    ret(count);
        if(count)
            vkEnumeratePhysicalDeviceGroups(inst, &count, ret.data());
        return ret;
    }

#if 0
    // not in the SDK?
    std::vector<VkPhysicalDeviceGroupProperties>    vqEnumeratePhysicalDeviceGroupsKHR(VkInstance inst)
    {
        uint32_t    count   = 0;
        vkEnumeratePhysicalDeviceGroupsKHR(inst, &count, nullptr);
        std::vector<VkPhysicalDeviceGroupProperties>    ret(count);
        if(count)
            vkEnumeratePhysicalDeviceGroupsKHR(inst, &count, ret.data());
        return ret;
    }
#endif

    std::vector<VkPhysicalDevice>        vqEnumeratePhysicalDevices(VkInstance inst)
    {
        uint32_t    count   = 0;
        vkEnumeratePhysicalDevices(inst, &count, nullptr);
        std::vector<VkPhysicalDevice>    ret(count);
        if(count)
            vkEnumeratePhysicalDevices(inst, &count, ret.data());
        return ret;
    }


    Result<uint32_t>                vqFindFirstGraphicsQueue(const std::vector<VkQueueFamilyProperties>&queues)
    {
        for(uint32_t i=0;i<queues.size();++i)
            if(queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                return { i, true };
        return {};
    }

    Result<uint32_t>                vqFindFirstPresentQueue(VkPhysicalDevice dev, VkSurfaceKHR srf)
    {
        uint32_t        count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(dev,&count,nullptr);
        for(uint32_t i=0;i<count;++i){
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(dev, i, srf, &presentSupport);
            if(presentSupport)
                return { i, true };
        }
        return {};
    }


    std::vector<VkQueueFamilyProperties> vqGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice dev)
    {
        uint32_t        count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(dev,&count,nullptr);
        std::vector<VkQueueFamilyProperties> ret(count);
        vkGetPhysicalDeviceQueueFamilyProperties(dev,&count,ret.data());
        return ret;
    }

    std::vector<const char*>             vqGlfwRequiredExtensions()
    {
        uint32_t    cnt = 0;
        const char**    them    = glfwGetRequiredInstanceExtensions(&cnt);
        return std::vector<const char*>(them, them+cnt);
    }


    std::set<std::string>                     vqNameSet(const std::vector<VkExtensionProperties>& props)
    {
        std::set<std::string> ret;
        for(const auto& v : props)
            ret << std::string(v.extensionName);
        return ret;
    }

    std::set<std::string>                     vqNameSet(const std::vector<VkLayerProperties>& props)
    {
        std::set<std::string> ret;
        for(const auto& v : props)
            ret << std::string(v.layerName);
        return ret;
    }


    VkQueueFlags                    vqOrFlags(const std::vector<VkQueueFamilyProperties>& queues)
    {
        VkQueueFlags    ret{0};
        for(const auto& q : queues)
            ret |= q.queueFlags;
        return ret;
    }
}

