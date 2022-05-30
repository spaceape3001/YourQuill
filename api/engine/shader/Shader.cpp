////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ShaderInfoWriter.hpp"
#include <basic/DelayInit.hpp>
#include <basic/FileUtils.hpp>

YQ_OBJECT_IMPLEMENT(yq::Shader)
YQ_OBJECT_IMPLEMENT(yq::ShaderCache)
YQ_OBJECT_IMPLEMENT(yq::ShaderCompiler)
YQ_OBJECT_IMPLEMENT(yq::ShaderLoader)

namespace yq {


    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    ShaderInfo::ShaderInfo(std::string_view zName, const AssetInfo& base, const std::source_location& sl) :
        AssetInfo(zName, base, sl)
    {
        set_option(SHADER);
    }

    Result<ByteArray>  Shader::compile(const std::filesystem::path& source, const std::filesystem::path& target)
    {
        //  TODO
        return {};
    }

    Ref<Shader>     Shader::get(const std::filesystem::path&fp)
    {
        return ShaderCache::singleton().get(fp);
    }
    
    Ref<Shader>     Shader::get(uint64_t i)
    {
        return ShaderCache::singleton().get(i);
    }

        //! Used to "validate" a shader
    Result<ByteArray>   Shader::validate(const std::filesystem::path& source)
    {
        //  TODO
        return {};
    }

    ////////////////////////////////////////////////////////////////////////////////

    size_t  Shader::data_size() const 
    {
        return m_payload.size();
    }

    bool    Shader::save_binary(const std::filesystem::path& fp) const 
    {
        return file_write(fp, m_payload);
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    ShaderCacheInfo::ShaderCacheInfo(std::string_view zName, const AssetCacheInfo& base, const std::source_location& sl) :
        AssetCacheInfo(zName, base, sl)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////

    ShaderCache&     ShaderCache::singleton()
    {
        static ShaderCache* s_singleton = new ShaderCache;
        return *s_singleton;
    }

    ////////////////////////////////////////////////////////////////////////////////

    ShaderCache::ShaderCache()
    {
    }
    
    ShaderCache::~ShaderCache()
    {
    }

    Ref<Shader>   ShaderCache::get(const std::filesystem::path& fp)
    {
        Ref<Asset>  ret = AssetCache::get(fp);
        return Ref<Shader>(static_cast<Shader*>(ret.ptr()));
    }
    
    Ref<Shader>   ShaderCache::get(uint64_t i)
    {
        Ref<Asset>  ret = AssetCache::get(i);
        return Ref<Shader>(static_cast<Shader*>(ret.ptr()));
    }

    Ref<Asset>      ShaderCache::load_binary(const std::filesystem::path& fp) const
    {
        ByteArray   bytes   = file_bytes(fp);
        if(bytes.empty())
            return Ref<Asset>();
        
        Shader* sc  = new Shader;
        sc -> m_payload = std::move(bytes);
        return Ref<Asset>(sc);
    }
    

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    ShaderCompilerInfo::ShaderCompilerInfo(std::string_view zName, const AssetCompilerInfo& base, const std::source_location& sl) :
        AssetCompilerInfo(zName, base, sl)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    ShaderLoaderInfo::ShaderLoaderInfo(std::string_view zName, const AssetLoaderInfo& base, const std::source_location& sl) :
        AssetLoaderInfo(zName, base, sl)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    
    class GLSLCompiler : public ShaderCompiler {
        YQ_OBJECT_DECLARE(GLSLCompiler, ShaderCompiler)
    public:
        bool    compile(const std::filesystem::path& source, const std::filesystem::path& target) const 
        {
            return Shader::compile(source, target).good;
        }
    };
    
    
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    
    YQ_INVOKE(
        auto si = writer<Shader>();
        si.abstract();
        
        auto sca = writer<ShaderCache>();
        sca.abstract();

        auto scc = writer<ShaderCompiler>();
        scc.abstract();

        auto sl = writer<ShaderLoader>();
        sl.abstract();
        
        auto glsl = writer<GLSLCompiler>();
        glsl.description("Shader compiler for GLSL language");
    )
}

YQ_OBJECT_IMPLEMENT(yq::GLSLCompiler)



