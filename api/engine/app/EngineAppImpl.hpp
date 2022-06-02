////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EngineAppImpl.hpp"
#include <vulkan/vulkan_core.h>
#include <set>
#include <vector>

namespace yq {

    struct EngineApp::Impl {
        EngineCreateInfo                    appInfo;
        VkInstance                          instance    = nullptr;
        bool                                glfw        = false;
        std::vector<VkLayerProperties>      allLayerProps;
        std::vector<VkExtensionProperties>  allExtensionProps;
        std::set<std::string>               allLayerNames;
        std::set<std::string>               allExtensionNames;
        std::vector<const char*>            extensions;
        std::vector<const char*>            layers;
        
        Impl(const EngineCreateInfo&);
        ~Impl();
        
        bool        init();
        void        kill();
    };

}
