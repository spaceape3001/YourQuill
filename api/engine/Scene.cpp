////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Scene.hpp"
#include "Camera.hpp"

namespace yq {
    namespace asset {
        const Camera*   Scene::camera(uint64_t i) const
        {
            if(cameras.empty())
                return nullptr;
            if(!i)
                return cameras.front().ptr();
            for(auto& c : cameras)
                if(c -> id() == i)
                    return c.ptr();
            return nullptr;
        }
    }
}
