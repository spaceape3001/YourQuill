////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/asset/Asset.hpp>

namespace yq {
    namespace engine {
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
        
        protected:
            Texture();
            ~Texture();
        };

        template <typename C>
        class TextureInfo::Writer : public AssetInfo::Writer<C> {
        public:
            Writer(TextureInfo* textureInfo) : AssetInfo::Writer<C>(textureInfo)
            {
            }
            
            Writer(TextureInfo& textureInfo) : Writer(&textureInfo)
            {
            }
        };
    }
}
