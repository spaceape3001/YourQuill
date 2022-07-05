////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <engine/ViewerCreateInfo.hpp>

struct GLFWwindow;

namespace yq {
    namespace engine {
        class Visualizer;
        class VqWindow : trait::not_copyable {
        public:

            VqWindow(){}
            explicit VqWindow(Visualizer&, const ViewerCreateInfo& wci=ViewerCreateInfo());
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
}
