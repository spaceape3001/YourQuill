////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/asset/Asset.hpp>

namespace yq {
    class Texture;
    class TextureLoader;
    class TextureCache;

    class TextureInfo : public AssetInfo {
    public:
        template <typename C> class Writer;

        TextureInfo(std::string_view zName, const AssetInfo& base, const std::source_location& sl=std::source_location::current());
        
    protected:
    };
    
    class Texture : public Asset {
        YQ_OBJECT_INFO(TextureInfo)
        YQ_OBJECT_DECLARE(Texture, Asset)
    public:
    };

}
