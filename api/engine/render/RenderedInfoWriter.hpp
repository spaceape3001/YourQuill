////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/meta/ObjectInfoWriter.hpp>
#include <engine/render/Rendered.hpp>
#include <engine/render/Pipeline.hpp>
#include <engine/render/PipelineWriter.hpp>
#include <basic/Mutable.hpp>
#include <variant>

namespace yq {
    namespace engine {
    
        template <typename C>
        class TypedPipeline : public Pipeline {
        public:
            class Builder;
            
            TypedPipeline(std::string_view name, RenderedInfo* parent, const std::source_location&sl) : Pipeline(name, parent, sl)
            {
            }
        };
        
        template <typename C>
        class TypedPipeline<C>::Builder : public Pipeline::Writer {
        public:
            
            Builder(TypedPipeline<C>*p) : Pipeline::Writer(p), m_myPipeline(p) 
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
            
            
            TypedPipeline<C>::Builder   pipeline(std::string_view name=std::string_view(), const std::source_location&sl=std::source_location::current())
            {
                TypedPipeline<C>*       tp  = new TypedPipeline<C>( name, m_rendered, sl);
                if(m_rendered && !m_rendered->m_primary)
                    m_rendered->m_primary   = tp;
                return typename TypedPipeline<C>::Builder(tp);
            }
            
        private:
            RenderedInfo*       m_rendered;
        };
    }
}
