////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Object.hpp>
#include <basic/meta/ObjectInfo.hpp>
#include <engine/pipeline/PipelineConfig.hpp>

namespace yq {
    namespace engine {
        class Renderable;
        
        
        
        class RenderableInfo : public ObjectInfo {
        public:
            template <typename C> struct Writer;
            
            const PipelineConfig*   pipeline(std::string_view) const;


            RenderableInfo(std::string_view, ObjectInfo&, const std::source_location& sl = std::source_location::current());
            
        private:
            std::map<std::string, PipelineConfig*>   m_pipelines;
        };


        /*! \brief Shows up on the screen
        
            If you want it to show up on the viewport, it needs to be renderable, and thus derived
            from this thing.
            
        */
        class Renderable : public Object {
            YQ_OBJECT_INFO(RenderableInfo);
            YQ_OBJECT_DECLARE(Renderable, Object)
        public:    
        
        };

    }
}
