////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/meta/Meta.hpp>
#include <engine/EngineConfig.hpp>
#include <engine/render/PipelineConfig.hpp>

namespace yq {
    template <size_t> class BasicBuffer;

    namespace engine {
        class Rendered;
        class RenderedInfo;
        
        /*
            The pipeline is a vulkan concept, however, we need it here.  Config remains as is, however, we need to 
            track the configurations, make it easy to build, and allow for rendereds to do it seamlessly.   
            For VBOs, we will discourage (but not prohibt) custom types.
            
            struct Sample : public Rendered {
                std::array<glm::vec3, 3>            staticCorners;
                Mutable<std::array<glm::vec3, 3>>   dynamicCorners;
            };
            
            With syntax like.... (about as complex as I'd like it to be)
            
            p = writer<SampeT>.pipeline();
            p.vbo(&Sample::staticCorners);
            
            Once built, this thing is expected to be immutable & fixed.
        */
        class Pipeline : public Meta {
        public:
            
            class Writer;
        
            const RenderedInfo*     rendered() const { return m_rendered; }
            
            bool                    has_push() const { return m_config.push.type != PushConfigType::None; }
            virtual void            get_push(const Rendered*, BasicBuffer<MAX_PUSH>&) const {}
            
            const PipelineConfig&   config() const { return m_config; }
        
        protected:
            Pipeline(std::string_view, const std::source_location& sl);
            Pipeline(std::string_view, RenderedInfo* parent, const std::source_location& sl);
            
            PipelineConfig          m_config;
            const RenderedInfo*     m_rendered    = nullptr;
        };
        
    }
}
