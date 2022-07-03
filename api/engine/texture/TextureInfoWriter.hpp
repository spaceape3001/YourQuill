////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/asset/AssetInfoWriter.hpp>
#include <engine/texture/Texture.hpp>
#include <engine/texture/TextureCache.hpp>
#include <engine/texture/TextureLoader.hpp>

namespace yq {
    namespace engine {

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
