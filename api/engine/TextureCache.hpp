////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/ResourceCache.hpp>

namespace yq {
    class Texture;
    class TextureLoader;
    class TextureCache;

    
    class TextureCache : public ResourceCache {
        YQ_OBJECT_DECLARE(TextureCache, ResourceCache)
    public:
    
    private:
        TextureCache();
        ~TextureCache();
    };
}
