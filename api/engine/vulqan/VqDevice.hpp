////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VqQueues.hpp"

#include <basic/trait/not_copyable.hpp>
#include <vulkan/vulkan_core.h>
#include <variant>
#include <vector>

namespace yq {
    class VqSurface;
    
    using VqQueueSpec = std::variant<std::monostate, uint32_t, std::vector<float>>;
    
    class VqDevice : trait::not_copyable {
    public:
    
        struct Config {
            std::vector<const char*>    extensions;
            VqQueueSpec                 graphics;
            VqQueueSpec                 present;
            
            Config(){}
        };
    
        VqDevice(){}
        explicit VqDevice(const VqSurface&, const Config& dci=Config());
        VqDevice(VqDevice&&);
        VqDevice& operator=(VqDevice&&);
        ~VqDevice();
        
        bool    good() const noexcept { return m_device != nullptr; }
        operator bool() const noexcept { return good(); }
        operator VkDevice() const noexcept { return m_device; }
        VkDevice device() const noexcept { return m_device; }
        
        VqQueues&   graphics() { return m_graphics; }
        VqQueues&   present() { return m_present; }

        const VqQueues&   graphics() const { return m_graphics; }
        const VqQueues&   present() const { return m_present; }

        VkQueue     graphics(uint32_t) const;
        VkQueue     present(uint32_t) const;
    
    private:
        void    dtor();
        void    move(VqDevice&&);
        
        VkDevice    m_device    = nullptr;
        VqQueues    m_graphics, m_present;
    };
}
