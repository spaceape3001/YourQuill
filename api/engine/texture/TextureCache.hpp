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

    
    class TextureCacheInfo : public AssetCacheInfo {
    public:
        template <typename C> class Writer;

        TextureCacheInfo(std::string_view zName, const AssetCacheInfo& base, const std::source_location& sl=std::source_location::current());

    protected:
    };
    
    class TextureCache : public AssetCache {
        YQ_OBJECT_INFO(TextureCacheInfo)
        YQ_OBJECT_DECLARE(TextureCache, AssetCache)
    public:
    
    private:
        TextureCache();
        ~TextureCache();
    };

}
