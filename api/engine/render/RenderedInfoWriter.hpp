////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/meta/ObjectInfoWriter.hpp>
#include <engine/render/Rendered.hpp>
#include <engine/render/Pipeline.hpp>
#include <basic/Mutable.hpp>
#include <variant>

namespace yq {
    namespace engine {
        template <typename C>
        class Rendered::TypedPipeline : public Pipeline {
        public:
            class Builder;
            
            TypedPipeline(std::string_view name, RenderedInfo* parent, const std::source_location&sl) :
                Pipeline(name, parent, sl)
            {
            }
        };
        
        template <typename C>
        class Rendered::TypedPipeline<C>::Builder : public Meta::Writer {
        public:
            
            Builder(TypedPipeline<C>*p) : Meta::Writer(p), m_myPipeline(p) 
            {
            }
            
            //template <typename T>
            //VBOWriter<T>        vbo(T C::*p, uint32_t);
        
        private:
            TypedPipeline<C>*   m_myPipeline = nullptr;
        };
    
    
        template <typename C>
        class RenderedInfo::Writer : public ObjectInfo::Writer<C> {
        public:
            Writer(RenderedInfo* renderInfo) : ObjectInfo::Writer<C>(renderInfo), m_rendered(renderInfo)
            {
            }
            
            Writer(RenderedInfo& renderInfo) : Writer(&renderInfo)
            {
            }
            
            
            Rendered::TypedPipeline<C>::Builder   pipeline(std::string_view name=std::string_view(), const std::source_location&sl=std::source_location::current())
            {
                Rendered::TypedPipeline<C>*       tp  = new Rendered::TypedPipeline<C>( name, m_rendered, sl);
                if(m_rendered && !m_rendered->m_primary)
                    m_rendered->m_primary   = tp;
                return typename Rendered::template TypedPipeline<C>::Builder(tp);
            }
            
        private:
            RenderedInfo*       m_rendered;
        };
    }
}
