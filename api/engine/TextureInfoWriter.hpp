////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/AssetInfoWriter.hpp>
#include <engine/Texture.hpp>
#include <engine/TextureCache.hpp>
#include <engine/TextureLoader.hpp>

namespace yq {
    namespace engine {
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

        template <typename C>
        class TextureLoaderInfo::Writer : public AssetLoaderInfo::Writer<C> {
        public:
            Writer(TextureLoaderInfo* textureLoaderInfo) : AssetLoaderInfo::Writer<C>(textureLoaderInfo)
            {
            }
            
            Writer(TextureLoaderInfo& textureLoaderInfo) : Writer(&textureLoaderInfo)
            {
            }
        };
    }
}
