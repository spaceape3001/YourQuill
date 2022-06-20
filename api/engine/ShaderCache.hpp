////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/ResourceCache.hpp>
#include <engine/ShaderPointer.hpp>
#include <basic/Ref.hpp>

namespace yq {
    class Shader;
    class ShaderCache;

    class ShaderCache : public ResourceCache {
        YQ_OBJECT_DECLARE(ShaderCache, ResourceCache)
    public:
    
        static ShaderCache&     singleton();
        
        ShaderPtr     get(const std::filesystem::path&);
        ShaderPtr     get(uint64_t);
    
    protected:
        Ref<const Resource>      load_binary(const std::filesystem::path&) const override;
    
    private:
        ShaderCache();
        ~ShaderCache();
    };

}
