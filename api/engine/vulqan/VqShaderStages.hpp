////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/shader/ShaderPointer.hpp>
#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <vulkan/vulkan_core.h>

namespace yq {
    class VqWindow;
    class Shader;
    
    class VqShaderStages : trait::not_copyable, trait::not_moveable {
    public:
    
        VqShaderStages(VqWindow&, const std::vector<ShaderPtr>&);
        VqShaderStages(VkDevice, const std::vector<ShaderPtr>&);
        ~VqShaderStages();
        
        uint32_t        mask() const { return m_mask; }
        
        friend VkGraphicsPipelineCreateInfo&  operator<<(VkGraphicsPipelineCreateInfo&, const VqShaderStages&);
        
    private:
        std::vector<VkPipelineShaderStageCreateInfo>    m_stages;
        VkDevice                                        m_device    = nullptr;
        uint32_t                                        m_mask      = 0;
        
    };
}

