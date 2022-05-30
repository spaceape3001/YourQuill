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

    class ShaderCacheInfo : public AssetCacheInfo {
    public:
        template <typename C> class Writer;

        ShaderCacheInfo(std::string_view zName, const AssetCacheInfo& base, const std::source_location& sl=std::source_location::current());

    protected:
    };
    
    class ShaderCache : public AssetCache {
        YQ_OBJECT_INFO(ShaderCacheInfo)
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
