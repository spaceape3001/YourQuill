////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/asset/AssetCache.hpp>
#include <basic/Ref.hpp>

namespace yq {
    class Shader;
    class ShaderCache;

    class ShaderCache : public AssetCache {
        YQ_OBJECT_DECLARE(ShaderCache, AssetCache)
    public:
    
        static ShaderCache&     singleton();
        
        Ref<Shader>     get(const std::filesystem::path&);
        Ref<Shader>     get(uint64_t);
    
    protected:
        Ref<Asset>      load_binary(const std::filesystem::path&) const override;
    
    private:
        ShaderCache();
        ~ShaderCache();
    };

}
