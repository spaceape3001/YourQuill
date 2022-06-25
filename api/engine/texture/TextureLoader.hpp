////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/resource/ResourceLoader.hpp>
#include <basic/Ref.hpp>

namespace yq {
    namespace engine {
        class Texture;
        class TextureLoader;
        class TextureCache;
        
        class TextureLoaderInfo : public ResourceLoaderInfo {
        public:
            template <typename C> class Writer;

            TextureLoaderInfo(std::string_view zName, const ResourceLoaderInfo& base, const std::source_location& sl=std::source_location::current());

        protected:
        };
        
        class TextureLoader : public ResourceLoader {
            YQ_OBJECT_INFO(TextureLoaderInfo)
            YQ_OBJECT_DECLARE(TextureLoader, ResourceLoader)
        public:
        
        };

    }
}
