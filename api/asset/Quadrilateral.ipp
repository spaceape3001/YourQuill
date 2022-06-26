////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Quadrilateral.hpp"
#include <math/QuadrilateralData.hpp>
#include <math/shape_math.hpp>
#include <engine/render/RenderWriter.hpp>
#include <engine/shader/Shader.hpp>
#include <basic/preamble.hpp>

namespace yq {
    namespace asset {
        void Quadrilateral::initInfo()
        {
            auto w = writer<Quadrilateral>();
            auto p = w.pipeline();
            
            p.shader("assets/colored.vert");
            p.shader("assets/colored.frag");

            p.static_vertex(&Quadrilateral::m_vertex, "vertex"sv)
                .attribute(&ColorVertexData::position)
                .attribute(&ColorVertexData::color)
            ;
            
            p.static_index(&Quadrilateral::m_index, "index"sv);
            
            p.push(yq::engine::PushConfigType::Full);
        }

        Quadrilateral::Quadrilateral(const QuadrilateralData<ColorVertex2D>&tri)
        {
            m_vertex[0] = tri.a;
            m_vertex[1] = tri.b;
            m_vertex[2] = tri.c;
            m_vertex[3] = tri.d;
            
            m_index     = { 0, 1, 2, 2, 3, 0 };
            
            m_draw.index_count  = 6;
            m_draw.vertex_count  = 4;
        }
        
        Quadrilateral::~Quadrilateral()
        {
        }
        
        YQ_INVOKE(
            Quadrilateral::initInfo();
        );
    }
}
YQ_OBJECT_IMPLEMENT(yq::asset::Quadrilateral)
