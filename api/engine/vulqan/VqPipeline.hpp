////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Ref.hpp>

namespace yq {
    
    class VqWindow;
    class Shader;
    
    class VqPipeline {
    public:
        
            //   TODO...add specialization info...
            
        VqPipeline(VkDevice, const std::vector<Ref<const Shader>>&);
        VqPipeline(VqWindow&, const std::vector<Ref<const Shader>>&);
        ~VqPipeline();
    };

}

