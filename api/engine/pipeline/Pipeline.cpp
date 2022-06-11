////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <basic/meta/Init.hpp>
#include "PipelineBuilder.hpp"
#include "CullMode.hpp"
#include "FrontFace.hpp"
#include "PolygonMode.hpp"
#include "Topology.hpp"
#include <engine/shader/Shader.hpp>

YQ_TYPE_IMPLEMENT(yq::engine::CullMode)
YQ_TYPE_IMPLEMENT(yq::engine::FrontFace)
YQ_TYPE_IMPLEMENT(yq::engine::PolygonMode)
YQ_TYPE_IMPLEMENT(yq::engine::Topology)

namespace yq {
    namespace engine {
        void    PipelineBuilder::add_shader(ShaderSpec ss)
        {
            m_config.shaders.push_back(ss);
        }
        
        void    PipelineBuilder::add_shaders(std::initializer_list<ShaderSpec>sspecs)
        {
            for(const ShaderSpec& ss : sspecs)
                m_config.shaders.push_back(ss);
        }
        
        void    PipelineBuilder::topology(Topology v)
        {
            m_config.topology = v;
        }
        
        void    PipelineBuilder::polygons(PolygonMode v)
        {
            m_config.polymode   = v;
        }
        
        void    PipelineBuilder::front(FrontFace v)
        {
            m_config.front = v;
        }
        
        void    PipelineBuilder::culling(CullMode v)
        {
            m_config.culling    = v;
        }
        
        const PipelineConfig*   PipelineBuilder::copy() const
        {
            return new PipelineConfig(m_config);
        }

    }
}

