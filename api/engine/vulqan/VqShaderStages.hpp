////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/preamble.hpp>
#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <vulkan/vulkan_core.h>
#include <vector>

namespace yq {
    namespace engine {
        class Shader;
        class Viewer;
        
        class VqShaderStages : trait::not_copyable, trait::not_moveable {
        public:
        
            VqShaderStages(Viewer&, const std::vector<ShaderSpec>&);
            VqShaderStages(VkDevice, const std::vector<ShaderSpec>&);
            ~VqShaderStages();
            
            uint32_t        mask() const { return m_mask; }
            
            friend VkGraphicsPipelineCreateInfo&  operator<<(VkGraphicsPipelineCreateInfo&, const VqShaderStages&);
            
        private:
            std::vector<VkPipelineShaderStageCreateInfo>    m_stages;
            VkDevice                                        m_device    = nullptr;
            uint32_t                                        m_mask      = 0;
            
        };
    }

}
