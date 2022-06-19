////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Renderable.hpp"
#include "RenderableInfoWriter.hpp"
#include "RenderObject.hpp"
#include "RenderObjectInfoWriter.hpp"
#include <basic/DelayInit.hpp>
#include <basic/meta/Init.hpp>
#include <math/tensor_math.hpp>
#include <math/vec/Tensor33.hpp>
#include <math/vec/Tensor44.hpp>

namespace yq {
    namespace engine {
        RenderableInfo::RenderableInfo(std::string_view name, ObjectInfo& base, const std::source_location& sl) : 
            ObjectInfo(name, base, sl)
        {
        }

        RenderObjectInfo::RenderObjectInfo(std::string_view name, ObjectInfo& base, const std::source_location& sl) : 
            RenderableInfo(name, base, sl)
        {
        }
        
        RenderObject::RenderObject()    = default;
        RenderObject:: ~RenderObject()  = default;

        Tensor44D   RenderObject::calc_model_matrix() const
        {
            Tensor33D   T   = tensor(m_orientation.value) * diagonal(m_scale.value);
            return Tensor44D(
                T.xx, T.xy, T.xz, m_position.value.x,
                T.yx, T.yy, T.yz, m_position.value.y,
                T.zx, T.zy, T.zz, m_position.value.z,
                0., 0., 0., 1.
            );
        }

        Tensor44F   RenderObject::calc_model_matrix_float() const
        {
            Tensor33D   T   = tensor(m_orientation.value) * diagonal(m_scale.value);
            return Tensor44F(
                (float) T.xx, (float) T.xy, (float) T.xz, (float) m_position.value.x,
                (float) T.yx, (float) T.yy, (float) T.yz, (float) m_position.value.y,
                (float) T.zx, (float) T.zy, (float) T.zz, (float) m_position.value.z,
                0.f, 0.f, 0.f, 1.f
            );
        }

        void        RenderObject::set_bounds(const AxBox3D&v)
        {
            m_bounds    = v;
        }

        void        RenderObject::set_orientation(const Quaternion3D&v)
        {
            m_orientation = v;
        }

        void        RenderObject::set_position(const Vector3D&v)
        {
            m_position  = v;
        }
        
        void        RenderObject::set_scale(const Vector3D&v)
        {
            m_scale     = v;
        }
        
        YQ_INVOKE(
            auto robj   = writer<RenderObject>();
            robj.property("pos", &RenderObject::position).setter(&RenderObject::set_position);
            robj.property("scale", &RenderObject::scale).setter(&RenderObject::set_scale);
            robj.property("ori", &RenderObject::orientation).setter(&RenderObject::set_orientation);
            robj.property("bounds", &RenderObject::bounds).setter(&RenderObject::set_bounds);
        )
    }
}

YQ_OBJECT_IMPLEMENT(yq::engine::RenderObject)
YQ_OBJECT_IMPLEMENT(yq::engine::Renderable)
