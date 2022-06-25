////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/asset/AssetLoader.hpp>
#include <basic/Ref.hpp>

namespace yq {
    namespace engine {
        class Texture;
        class TextureLoader;
        class TextureCache;
        
        class TextureLoaderInfo : public AssetLoaderInfo {
        public:
            template <typename C> class Writer;

            TextureLoaderInfo(std::string_view zName, const AssetLoaderInfo& base, const std::source_location& sl=std::source_location::current());

        protected:
        };
        
        class TextureLoader : public AssetLoader {
            YQ_OBJECT_INFO(TextureLoaderInfo)
            YQ_OBJECT_DECLARE(TextureLoader, AssetLoader)
        public:
        
        };

    }
}
