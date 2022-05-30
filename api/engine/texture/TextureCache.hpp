////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/asset/AssetCache.hpp>

namespace yq {
    class Texture;
    class TextureLoader;
    class TextureCache;

    
    class TextureCache : public AssetCache {
        YQ_OBJECT_DECLARE(TextureCache, AssetCache)
    public:
    
    private:
        TextureCache();
        ~TextureCache();
    };

}
