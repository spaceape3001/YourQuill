////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/asset/AssetCache.hpp>
#include <engine/shader/ShaderPointer.hpp>
#include <basic/Ref.hpp>

namespace yq {
    class Shader;
    class ShaderCache;

    class ShaderCache : public AssetCache {
        YQ_OBJECT_DECLARE(ShaderCache, AssetCache)
    public:
    
        static ShaderCache&     singleton();
        
        ShaderPtr     get(const std::filesystem::path&);
        ShaderPtr     get(uint64_t);
    
    protected:
        Ref<const Asset>      load_binary(const std::filesystem::path&) const override;
    
    private:
        ShaderCache();
        ~ShaderCache();
    };

}
