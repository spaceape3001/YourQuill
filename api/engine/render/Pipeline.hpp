////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    namespace engine {
        /*
            The pipeline is a vulkan concept, however, we need it here.  Config remains as is, however, we need to 
            track the configurations, make it easy to build, and allow for rendereds to do it seamlessly.   
            For VBOs, we will discourage (but not prohibt) custom types.
            
            struct Sample : public Rendered {
                std::array<glm::vec3, 3>            staticCorners;
                Mutable<std::array<glm::vec3, 3>>   dynamicCorners;
            };
            
            With syntax like.... (about as complex as I'd like it to be)
            
            writer<SampeT>.vbo(&Sample::staticCorners);
            
            Once built, this thing is expected to be immutable.
        */
        
        
        
    }
}
