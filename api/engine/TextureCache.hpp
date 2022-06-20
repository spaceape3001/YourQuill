////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/AssetCache.hpp>

namespace yq {
    namespace engine {
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
}
