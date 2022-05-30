////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/asset/Asset.hpp>
#include <basic/ByteArray.hpp>

namespace yq {
    class Shader;
    class ShaderCompiler;
    class ShaderLoader;
    class ShaderCache;

    class ShaderInfo : public AssetInfo {
    public:
        template <typename C> class Writer;

        ShaderInfo(std::string_view zName, const AssetInfo& base, const std::source_location& sl=std::source_location::current());
        
    protected:
    };
    
        //! Shader is a Vulkan compatible shader (compiled)
    class Shader : public Asset {
        YQ_OBJECT_INFO(ShaderInfo)
        YQ_OBJECT_DECLARE(Shader, Asset)
    public:

        //! Used to "compile" a shader
        static Result<ByteArray>        compile(const std::filesystem::path& source, const std::filesystem::path& target);
        //! Used to "validate" a shader
        static Result<ByteArray>        validate(const std::filesystem::path& source);

        static Ref<Shader>     get(const std::filesystem::path&);
        static Ref<Shader>     get(uint64_t);

        const ByteArray&    payload() const { return m_payload; }
        virtual size_t      data_size() const override;
        virtual bool        save_binary(const std::filesystem::path&) const override;
    
    private:
        friend class ShaderCache;
        ByteArray       m_payload;
    };
    

}
