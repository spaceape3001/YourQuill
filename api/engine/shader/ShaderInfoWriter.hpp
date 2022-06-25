////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/resource/ResourceInfoWriter.hpp>
#include <engine/shader/Shader.hpp>
#include <engine/shader/ShaderCache.hpp>
#include <engine/shader/ShaderCompiler.hpp>
#include <engine/shader/ShaderLoader.hpp>

namespace yq {
    namespace engine {
        template <typename C>
        class ShaderInfo::Writer : public ResourceInfo::Writer<C> {
        public:
            Writer(ShaderInfo* shaderInfo) : ResourceInfo::Writer<C>(shaderInfo)
            {
            }
            
            Writer(ShaderInfo& shaderInfo) : Writer(&shaderInfo)
            {
            }
        };


        template <typename C>
        class ShaderCompilerInfo::Writer : public ResourceCompilerInfo::Writer<C> {
        public:
            Writer(ShaderCompilerInfo* shaderCompilerInfo) : ResourceCompilerInfo::Writer<C>(shaderCompilerInfo)
            {
            }
            
            Writer(ShaderCompilerInfo& shaderCompilerInfo) : Writer(&shaderCompilerInfo)
            {
            }
        };

       template <typename C>
        class ShaderLoaderInfo::Writer : public ResourceLoaderInfo::Writer<C> {
        public:
            Writer(ShaderLoaderInfo* shaderLoaderInfo) : ResourceLoaderInfo::Writer<C>(shaderLoaderInfo)
            {
            }
            
            Writer(ShaderLoaderInfo& shaderLoaderInfo) : Writer(&shaderLoaderInfo)
            {
            }
        };

    }
}
