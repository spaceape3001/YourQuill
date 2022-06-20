////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <engine/WindowCreateInfo.hpp>

struct GLFWwindow;

namespace yq {
    class VqInternal;
    class VqWindow : trait::not_copyable {
    public:

        VqWindow(){}
        explicit VqWindow(VqInternal&, const WindowCreateInfo& wci=WindowCreateInfo());
        VqWindow(VqWindow&&);
        VqWindow& operator=(VqWindow&&);
        ~VqWindow();
        
        GLFWwindow* window() const noexcept { return m_window; }
        operator bool() const noexcept { return m_window != nullptr; }
        operator GLFWwindow*() const noexcept { return m_window; }
    
    
    private:
        void    dtor();
    
        GLFWwindow*     m_window = nullptr;
    };
}
