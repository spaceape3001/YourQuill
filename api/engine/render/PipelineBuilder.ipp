////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "PipelineBuilder.hpp"
#include "StdPushConstant.hpp"

#include <basic/meta/Init.hpp>
#include <basic/meta/TypeInfo.hpp>
#include <engine/shader/Shader.hpp>


namespace yq {
    namespace engine {

        PipelineBuilder::PipelineBuilder(PipelineConfig& cfg) : config(cfg)
        {
            for(auto& vbo : cfg.vbos)
                for(auto & a : vbo.attrs)
            {
                m_locations.insert(a.location);     // won't be perfect, should help though
            }
        }

        void    PipelineBuilder::culling(CullMode v)
        {
            config.culling    = v;
        }
        
        void    PipelineBuilder::front(FrontFace v)
        {
            config.front = v;
        }

        uint32_t    PipelineBuilder::location_filter(uint32_t loc, uint32_t req)
        {
            if(loc != UINT32_MAX){
                for(uint32_t i=loc; i<loc+req; ++i){
                    [[maybe_unused]] auto j = m_locations.insert(i);
                    assert(j.second && "Location already assigned!");
                }
                return loc;
            }
            
            if(m_locations.empty()){
                for(uint32_t i=0; i<req; ++i){
                    m_locations.insert(i);
                }
                return 0;
            }
            
            loc = *(m_locations.rbegin()) + 1;
            for(uint32_t i=0; i<req; ++i){
                m_locations.insert(i+loc);
            }
            return loc;
        }
        
        void    PipelineBuilder::polygons(PolygonMode v)
        {
            config.polymode   = v;
        }
        

        void    PipelineBuilder::push(PushConfig::Type v)
        {
            switch(v){
            case PushConfig::Full:
            case PushConfig::View:
                config.push.type  = v;
                config.push.size  = sizeof(StdPushConstant);
                break;
            default:
                break;
            }
        }

        void    PipelineBuilder::shader(ShaderSpec ss)
        {
            config.shaders.push_back(ss);
        }
        
        void    PipelineBuilder::shaders(std::initializer_list<ShaderSpec>sspecs)
        {
            for(const ShaderSpec& ss : sspecs)
                config.shaders.push_back(ss);
        }
        
            
        void    PipelineBuilder::topology(Topology v)
        {
            config.topology = v;
        }
        

    }
}
