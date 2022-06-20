////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/ResourceCompiler.hpp>
#include <basic/Ref.hpp>

namespace yq {
    class Shader;
    class ShaderCompiler;
    
    class ShaderCompilerInfo : public ResourceCompilerInfo {
    public:
        template <typename C> class Writer;

        ShaderCompilerInfo(std::string_view zName, const ResourceCompilerInfo& base, const std::source_location& sl=std::source_location::current());

    protected:
    };
    
    class ShaderCompiler : public ResourceCompiler {
        YQ_OBJECT_INFO(ShaderCompilerInfo)
        YQ_OBJECT_DECLARE(ShaderCompiler, ResourceCompiler)
    public:

    };

}
