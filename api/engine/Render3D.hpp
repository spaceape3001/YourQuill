////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Dirty.hpp>
#include <basic/Object.hpp>
#include <basic/Ref.hpp>
#include <basic/meta/ObjectInfo.hpp>
#include <engine/pipeline/PipelineConfig.hpp>
#include <engine/Rendered.hpp>
#include <math/SimpleSpace.hpp>
#include <math/shape/AxBox3.hpp>

namespace yq {
    namespace engine {
        class Render3D;
        
        class Render3DInfo : public RenderedInfo {
        public:
            template <typename C> struct Writer;

            Render3DInfo(std::string_view, ObjectInfo&, const std::source_location& sl = std::source_location::current());
        };


        /*! \brief Something that's rendered with a defined position, size, etc
        */
        class Render3D : public Rendered {
            YQ_OBJECT_INFO(Render3DInfo);
            YQ_OBJECT_DECLARE(Render3D, Rendered)
        public:    

            //  The model matrix in relation to its parent
            Tensor44D          calc_local() const;
            
            Render3D();
            virtual ~Render3D();
            
            const Vector3D&                 position() const { return m_space.value.position; }
            const Vector3D&                 scale() const { return m_space.value.scale; }
            const Quaternion3D&             orientation() const { return m_space.value.orientation; }
            const AxBox3D&                  bounds() const { return m_bounds.value; }
            
            void                            set_position(const Vector3D&);
            void                            set_scale(const Vector3D&);
            void                            set_orientation(const Quaternion3D&);
            void                            set_bounds(const AxBox3D&);
            
            virtual Ref<Render3D>           clone() const = 0;
            
        private:
            Render3D*                       m_parent    = nullptr;
            std::vector<Ref<Render3D>>      m_children;
            
            //! This is the coordinate space for the object (position, scale, & orientation)
            Dirty<SimpleSpace>              m_space;
            
            //! Bounding box of the render object in its x/y/z axis
            Dirty<AxBox3D>                  m_bounds        = { nan_v<AxBox3D> };
        };

    }
}
