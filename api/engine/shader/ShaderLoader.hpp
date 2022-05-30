////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/asset/AssetLoader.hpp>
#include <basic/Ref.hpp>

namespace yq {
    class Shader;
    class ShaderLoader;
    
    class ShaderLoaderInfo : public AssetLoaderInfo {
    public:
        template <typename C> class Writer;

        ShaderLoaderInfo(std::string_view zName, const AssetLoaderInfo& base, const std::source_location& sl=std::source_location::current());

    protected:
    };
    
    class ShaderLoader : public AssetLoader {
        YQ_OBJECT_INFO(ShaderLoaderInfo)
        YQ_OBJECT_DECLARE(ShaderLoader, AssetLoader)
    public:
    };

}
