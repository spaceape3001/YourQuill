////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/asset/AssetInfoWriter.hpp>
#include <engine/shader/Shader.hpp>
#include <engine/shader/ShaderCache.hpp>
#include <engine/shader/ShaderCompiler.hpp>
#include <engine/shader/ShaderLoader.hpp>

namespace yq {

    template <typename C>
    class ShaderInfo::Writer : public AssetInfo::Writer<C> {
    public:
        Writer(ShaderInfo* shaderInfo) : AssetInfo::Writer<C>(shaderInfo)
        {
        }
        
        Writer(ShaderInfo& shaderInfo) : Writer(&shaderInfo)
        {
        }
    };

    template <typename C>
    class ShaderCacheInfo::Writer : public AssetCacheInfo::Writer<C> {
    public:
        Writer(ShaderCacheInfo* shaderCacheInfo) : AssetCacheInfo::Writer<C>(shaderCacheInfo)
        {
        }
        
        Writer(ShaderCacheInfo& shaderCacheInfo) : Writer(&shaderCacheInfo)
        {
        }
    };

    template <typename C>
    class ShaderCompilerInfo::Writer : public AssetCompilerInfo::Writer<C> {
    public:
        Writer(ShaderCompilerInfo* shaderCompilerInfo) : AssetCompilerInfo::Writer<C>(shaderCompilerInfo)
        {
        }
        
        Writer(ShaderCompilerInfo& shaderCompilerInfo) : Writer(&shaderCompilerInfo)
        {
        }
    };

   template <typename C>
    class ShaderLoaderInfo::Writer : public AssetLoaderInfo::Writer<C> {
    public:
        Writer(ShaderLoaderInfo* shaderLoaderInfo) : AssetLoaderInfo::Writer<C>(shaderLoaderInfo)
        {
        }
        
        Writer(ShaderLoaderInfo& shaderLoaderInfo) : Writer(&shaderLoaderInfo)
        {
        }
    };

}
