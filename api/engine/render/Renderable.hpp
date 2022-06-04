////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Object.hpp>
#include <basic/meta/ObjectInfo.hpp>

namespace yq {
    namespace engine {
        class Renderable;
        
        class RenderableInfo : public ObjectInfo {
        public:
            template <typename C> struct Writer;
        };


        /*! \brief Shows up on the screen
        
            If you want it to show up on the viewport, it needs to be renderable, and thus derived
            from this thing.
            
        */
        class Renderable : public Object {
            YQ_OBJECT_INFO(RenderableInfo);
            YQ_OBJECT_DECLARE(Renderable, Object)
        public:    
        
        };

    }
}
