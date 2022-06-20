////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/ResourceLoader.hpp>
#include <basic/Ref.hpp>

namespace yq {
    class Shader;
    class ShaderLoader;
    
    class ShaderLoaderInfo : public ResourceLoaderInfo {
    public:
        template <typename C> class Writer;

        ShaderLoaderInfo(std::string_view zName, const ResourceLoaderInfo& base, const std::source_location& sl=std::source_location::current());

    protected:
    };
    
    class ShaderLoader : public ResourceLoader {
        YQ_OBJECT_INFO(ShaderLoaderInfo)
        YQ_OBJECT_DECLARE(ShaderLoader, ResourceLoader)
    public:
    };

}
