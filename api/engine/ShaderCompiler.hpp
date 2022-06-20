////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/AssetCompiler.hpp>
#include <basic/Ref.hpp>

namespace yq {
    namespace engine {
        class Shader;
        class ShaderCompiler;
        
        class ShaderCompilerInfo : public AssetCompilerInfo {
        public:
            template <typename C> class Writer;

            ShaderCompilerInfo(std::string_view zName, const AssetCompilerInfo& base, const std::source_location& sl=std::source_location::current());

        protected:
        };
        
        class ShaderCompiler : public AssetCompiler {
            YQ_OBJECT_INFO(ShaderCompilerInfo)
            YQ_OBJECT_DECLARE(ShaderCompiler, AssetCompiler)
        public:

        };

    }
}
