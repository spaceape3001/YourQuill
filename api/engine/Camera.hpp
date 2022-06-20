////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Object.hpp>
#include <basic/Ref.hpp>
#include <math/Size2.hpp>

#include <glm/glm.hpp>

namespace yq {
    namespace engine {

        class Camera;
        
        class CameraInfo : public ObjectInfo {
        public:
            template <typename C> struct Writer;
            
            CameraInfo(std::string_view, ObjectInfo&, const std::source_location& sl = std::source_location::current());
        };

        struct CameraProxy;


        struct CameraParams {
            Size2D          screen{};
        };


        /*! \brief Shows up on the screen
        
            If you want it to show up on the viewport, it needs to be renderable, and thus derived
            from this thing.
        */
        class Camera : public Object, public RefCount {
            YQ_OBJECT_INFO(CameraInfo);
            YQ_OBJECT_DECLARE(Camera, Object)
        public:    
        
            uint64_t        id() const { return m_id; }
            uint64_t        revision() const { return m_revision; }
        
            //! Returns the transform to go world -> screen space
            virtual glm::dmat4  world2screen(const CameraParams&) const = 0;
        
            CameraProxy     proxy(const CameraParams&) const;

        protected:
            Camera();
            Camera(const Camera&);
            ~Camera();
        
            void            changed();
        
        private:
            uint64_t                m_id        = 0;
            std::atomic<uint64_t>   m_revision{0};
        };
        
        using CameraPtr     = Ref<Camera>;
        using CameraCPtr    = Ref<const Camera>;


    }
}
