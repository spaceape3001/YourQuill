////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Dirty.hpp>
#include <basic/Object.hpp>
#include <basic/meta/ObjectInfo.hpp>
#include <engine/pipeline/PipelineConfig.hpp>
#include <engine/render/Renderable.hpp>
#include <math/shape/AxBox3.hpp>
#include <math/vec/Quaternion3.hpp>
#include <math/vec/Vector3.hpp>

namespace yq {
    namespace engine {
        class RenderObject;
        
        class RenderObjectInfo : public RenderableInfo {
        public:
            template <typename C> struct Writer;

            RenderObjectInfo(std::string_view, ObjectInfo&, const std::source_location& sl = std::source_location::current());
        };


        /*! \brief An object that'll show up on the screen
        */
        class RenderObject : public Renderable {
            YQ_OBJECT_INFO(RenderObjectInfo);
            YQ_OBJECT_DECLARE(RenderObject, Renderable)
        public:    

            
            Tensor44D          calc_model_matrix() const;
            Tensor44F          calc_model_matrix_float() const;
            
            RenderObject();
            virtual ~RenderObject();
            
            const Vector3D&     position() const { return m_position.value; }
            const Vector3D&     scale() const { return m_scale.value; }
            const Quaternion3D& orientation() const { return m_orientation.value; }
            const AxBox3D&      bounds() const { return m_bounds.value; }
            
            void                set_position(const Vector3D&);
            void                set_scale(const Vector3D&);
            void                set_orientation(const Quaternion3D&);
            void                set_bounds(const AxBox3D&);
            
            //! Center position of the render object
            Dirty<Vector3D>     m_position      = { 0., 0., 0. };
            //! Scale of the render object (along object's primary x/y/z axis)
            Dirty<Vector3D>     m_scale         = { 1., 1., 1. };
            //! Orientation of the render object with world space
            Dirty<Quaternion3D> m_orientation   = { 1., 0., 0., 0. };
            //! Bounding box of the render object in its x/y/z axis
            Dirty<AxBox3D>      m_bounds        = { nan_v<AxBox3D> };
        };

    }
}
