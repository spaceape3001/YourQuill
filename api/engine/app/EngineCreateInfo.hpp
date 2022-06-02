////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Required.hpp"
#include <basic/DbgRel.hpp>
#include <cstdint.hpp>
#include <string>

namespace yq {
    //! Info for initialization
    struct EngineCreateInfo {
    
        //! Application name
        std::string     app_name;
        
        //! Engine Name
        std::string     engine_name     = "(nameless)";
        
        //! Application version number
        uint32_t        app_version     = 0;
        
        //! Engine version number
        uint32_t        engine_version  = 0;
        
        //! Vulkan API version (zero will default to latest)
        uint32_t        vulkan_api      = 0;
        
        //! Add KHRONOS validation layer
        Required        validation      = YQ_DBGREL( Required::YES, Required::NO);
        
        
        EngineCreateInfo(){}
    };
}
