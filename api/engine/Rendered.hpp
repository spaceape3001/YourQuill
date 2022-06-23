////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Object.hpp>
#include <basic/meta/ObjectInfo.hpp>
#include <basic/UniqueID.hpp>
#include <basic/Revision.hpp>
#include <engine/PipelineConfig.hpp>

namespace yq {
    namespace engine {
        class Rendered;
        
        class RenderedInfo : public ObjectInfo {
        public:
            template <typename C> struct Writer;
            
            const PipelineConfig*   pipeline(std::string_view) const;


            RenderedInfo(std::string_view, ObjectInfo&, const std::source_location& sl = std::source_location::current());
            
        private:
            std::map<std::string, PipelineConfig*>   m_pipelines;
        };


        /*! \brief Shows up on the screen
        
            If you want it to show up on the viewport, it needs to be renderable, and thus derived
            from this thing.
        */
        class Rendered : public Object, public RefCount, public UniqueID, public Revision {
            YQ_OBJECT_INFO(RenderedInfo);
            YQ_OBJECT_DECLARE(Rendered, Object)
        public:    
            uint64_t        id() const { return UniqueID::id(); }
        
        protected:
            Rendered();
            Rendered(const Rendered&);
            ~Rendered();
        };

        using RenderedPtr   = Ref<Rendered>;

    }
}
