////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/AssetInfoWriter.hpp>
#include <engine/Shader.hpp>
#include <engine/ShaderCache.hpp>
#include <engine/ShaderCompiler.hpp>
#include <engine/ShaderLoader.hpp>

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
