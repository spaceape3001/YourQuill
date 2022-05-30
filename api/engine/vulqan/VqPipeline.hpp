////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Ref.hpp>

namespace yq {
    class VqWindow;
    struct PipelineConfig;
    
    class VqPipeline : public RefCount {
    public:
        
            //   TODO...add specialization info...
            
        VqPipeline(VqWindow&, const PipelineConfig&);
        ~VqPipeline();
        
    private:
        VkPipelineLayout        m_pipeline  = nullptr;
    };

}

