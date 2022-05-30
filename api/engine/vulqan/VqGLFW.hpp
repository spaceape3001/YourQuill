////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>

namespace yq {
    class VqGLFW : trait::not_copyable, trait::not_moveable {
    public:
    
        VqGLFW();
        ~VqGLFW();
        
        static bool     initialized();
    
    private:
        static VqGLFW* s_singleton;
    };
}

