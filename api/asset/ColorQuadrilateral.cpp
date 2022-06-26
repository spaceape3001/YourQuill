////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ColorQuadrilateral.hpp"
#include <math/QuadrilateralData.hpp>
#include <math/shape_math.hpp>
#include <engine/render/RenderWriter.hpp>
#include <engine/shader/Shader.hpp>
#include <basic/preamble.hpp>

namespace yq {
    namespace asset {
        void ColorQuadrilateral::initInfo()
        {
            auto w = writer<ColorQuadrilateral>();
            auto p = w.pipeline();
            
            p.shader("assets/colored.vert");
            p.shader("assets/colored.frag");

            p.static_vertex(&ColorQuadrilateral::m_vertex, "vertex"sv)
                .attribute(&ColorVertexData::position)
                .attribute(&ColorVertexData::color)
            ;
            
            p.static_index(&ColorQuadrilateral::m_index, "index"sv);
            
            p.push(yq::engine::PushConfigType::Full);
        }

        ColorQuadrilateral::ColorQuadrilateral(const QuadrilateralData<ColorVertex2D>&tri)
        {
            m_vertex[0] = tri.a;
            m_vertex[1] = tri.b;
            m_vertex[2] = tri.c;
            m_vertex[3] = tri.d;
            
            m_index     = { 0, 1, 2, 2, 3, 0 };
            
            m_draw.index_count  = 6;
            m_draw.vertex_count  = 4;
        }
        
        ColorQuadrilateral::~ColorQuadrilateral()
        {
        }
        
        YQ_INVOKE(
            ColorQuadrilateral::initInfo();
        );
    }
}
YQ_OBJECT_IMPLEMENT(yq::asset::ColorQuadrilateral)
