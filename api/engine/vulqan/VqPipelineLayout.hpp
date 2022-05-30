////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Ref.hpp>
#include <vulkan/vulkan_core.h>

namespace yq {
    class VqWindow;
    struct PipelineConfig;
    
    class VqPipelineLayout : public RefCount {
    public:
        
            //   TODO...add specialization info...
            
        VqPipelineLayout(VqWindow&, const PipelineConfig&);
        ~VqPipelineLayout();
        
        VkPipelineLayout    vulkan() const { return m_pipelineLayout; }
        
    private:
        VkPipelineLayout        m_pipelineLayout  = nullptr;
    };

}

