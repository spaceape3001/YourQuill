////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Rendered.hpp"
#include "RenderedInfoWriter.hpp"
#include <basic/DelayInit.hpp>
#include <basic/meta/Init.hpp>
#include <atomic>

namespace yq {
    namespace engine {

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        RenderedInfo::RenderedInfo(std::string_view name, ObjectInfo& base, const std::source_location& sl) : 
            ObjectInfo(name, base, sl)
        {
            set_option(RENDERED);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Rendered::Rendered()
        {
        }
        
        Rendered::Rendered(const Rendered&) : Rendered()
        {
        }

        Rendered::~Rendered()
        {
        }
        
        const Pipeline*   Rendered::pipeline() const
        {
            if(m_pipeline)
                return m_pipeline;
            return metaInfo().primary();
        }

        void              Rendered::set_wireframe(Tristate v)
        {
            m_wireframe = v;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        YQ_INVOKE(
            auto rend   = writer<Rendered>();
            rend.property("id", &Rendered::id);
        )
    }
}

YQ_OBJECT_IMPLEMENT(yq::engine::Rendered)
