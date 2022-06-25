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
#include <engine/enum/Tristate.hpp>
#include <engine/render/PipelineConfig.hpp>

namespace yq {
    namespace engine {
        class Rendered;
        class Pipeline;
        
        
        class RenderedInfo : public ObjectInfo {
        public:
            template <typename C> struct Writer;

            RenderedInfo(std::string_view, ObjectInfo&, const std::source_location& sl = std::source_location::current());
            const std::vector<const Pipeline*>&    pipelines() const { return m_pipelines; }
            const Pipeline*     pipeline(std::string_view) const;
            
            //! Primary pipeline (aka first created)
            const Pipeline*     primary() const { return m_primary; }
            
        private:
            const Pipeline*                 m_primary   = nullptr;
            std::vector<const Pipeline*>    m_pipelines;        // all of them
        };


        /*! \brief Shows up on the screen
        
            If you want it to show up on the viewport, it needs to be renderable, and thus derived
            from this thing.
        */
        class Rendered : public Object, public RefCount, public UniqueID, public Revision {
            YQ_OBJECT_INFO(RenderedInfo);
            YQ_OBJECT_DECLARE(Rendered, Object)
        public:    

            uint64_t            id() const { return UniqueID::id(); }
            const Pipeline*   pipeline() const;
            bool                is_culled() const { return m_culled; }
            
            Tristate            wireframe() const { return m_wireframe; }
            void                set_wireframe(Tristate);

        protected:
            Rendered();
            Rendered(const Rendered&);
            ~Rendered();
            
            // NULL defaults to primary pipeline
            const Pipeline*     m_pipeline      = nullptr;
            
            Tristate        m_wireframe;
            bool            m_culled    = false;
        };

        using RenderedPtr   = Ref<Rendered>;

    }
}
