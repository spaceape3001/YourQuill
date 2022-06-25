////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/resource/ResourceInfoWriter.hpp>
#include <engine/texture/Texture.hpp>
#include <engine/texture/TextureCache.hpp>
#include <engine/texture/TextureLoader.hpp>

namespace yq {
    namespace engine {
        template <typename C>
        class TextureInfo::Writer : public ResourceInfo::Writer<C> {
        public:
            Writer(TextureInfo* textureInfo) : ResourceInfo::Writer<C>(textureInfo)
            {
            }
            
            Writer(TextureInfo& textureInfo) : Writer(&textureInfo)
            {
            }
        };

        template <typename C>
        class TextureLoaderInfo::Writer : public ResourceLoaderInfo::Writer<C> {
        public:
            Writer(TextureLoaderInfo* textureLoaderInfo) : ResourceLoaderInfo::Writer<C>(textureLoaderInfo)
            {
            }
            
            Writer(TextureLoaderInfo& textureLoaderInfo) : Writer(&textureLoaderInfo)
            {
            }
        };
    }
}
