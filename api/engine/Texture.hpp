////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/Resource.hpp>

namespace yq {
    class Texture;
    class TextureLoader;
    class TextureCache;

    class TextureInfo : public ResourceInfo {
    public:
        template <typename C> class Writer;

        TextureInfo(std::string_view zName, const ResourceInfo& base, const std::source_location& sl=std::source_location::current());
        
    protected:
    };
    
    class Texture : public Resource {
        YQ_OBJECT_INFO(TextureInfo)
        YQ_OBJECT_DECLARE(Texture, Resource)
    public:
    };

}
