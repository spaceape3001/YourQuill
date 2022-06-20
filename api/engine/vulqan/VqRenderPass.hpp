////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <vulkan/vulkan_core.h>

namespace yq {
    class MainWindow;
    
    class VqRenderPass : trait::not_copyable {
    public:
        VqRenderPass(){}
        explicit VqRenderPass(VqInternal&);
        VqRenderPass(VqRenderPass&&);
        VqRenderPass& operator=(VqRenderPass&&);
        ~VqRenderPass();
        
        bool    good() const noexcept { return m_render != nullptr; }
        operator bool() const noexcept { return good(); }
        operator VkRenderPass() const noexcept { return m_render; }
        VkRenderPass render_pass() const noexcept { return m_render; }
    
    private:
        void    dtor();
        void    move(VqRenderPass&&);
        
        VkDevice        m_device    = nullptr;
        VkRenderPass    m_render = nullptr;
    };
}
