////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <basic/Enum.hpp>
#include <basic/Flag.hpp>
#include <basic/Meta/Meta.hpp>
#include <source_location>

namespace yq {
    namespace engine {
        //struct PipelineConfig {
            //// Going to have one or more choices in here
        //};
        
        class PipelineInfo : public Meta {
        public:
            struct Writer;
            
            const std::vector<std::string>& shaders() const { return m_shaders; }
            
        
        private:
            PipelineInfo(RenderableInfo*, std::string_view, const std::source_location& sl);
            std::vector<std::string>    m_shaders;
        };
        
    }
}
