////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ShaderInfoWriter.hpp"
#include <basic/DelayInit.hpp>
#include <basic/FileUtils.hpp>
#include <config/DirConfig.hpp>
#include <basic/Execute.hpp>
#include <engine/asset/ResultCC.hpp>

YQ_OBJECT_IMPLEMENT(yq::Shader)
YQ_OBJECT_IMPLEMENT(yq::ShaderCache)
YQ_OBJECT_IMPLEMENT(yq::ShaderCompiler)
YQ_OBJECT_IMPLEMENT(yq::ShaderLoader)

namespace yq {

    namespace {
        //  configuration to change/move
        std::filesystem::path   glsl_compiler_executable()
        {
            const char* z   = getenv("VULKAN_SDK_PATH");
            if(!z)
                z   = "/usr";
        
            return std::filesystem::path(z) / "bin" / "glslc";
        }

        std::filesystem::path   glsl_validator_executable()
        {
            const char* z   = getenv("VULKAN_SDK_PATH");
            if(!z)
                z   = "/usr";
        
            return std::filesystem::path(z) / "bin" / "glslangValidator";
        }
        
        path_vector_t       shader_dirs()
        {
            path_vector_t   ret;
            ret.push_back( std::filesystem::path(build::data_directory()) / "shaders" );
            return ret;
        }
        
        ResultCC    compile_shader(const ByteArray& input, const std::filesystem::path& source, const std::filesystem::path& target)
        {
            static const path_vector_t          dirs    = shader_dirs();
            static const std::filesystem::path  glslc   = glsl_compiler_executable();
            int                                 ecode   = -1;
            
            ProcessDescriptor   pd;
            pd.args.push_back(glslc.string());
            for(auto& d : dirs){
                pd.args.push_back("-I");
                pd.args.push_back(d.string());
            }
            
            pd.args.push_back("--target-env=vulkan1.2");
            
            pd.args.push_back("-x");        // GLSL is the language of choice
            pd.args.push_back("glsl");
            
            pd.args.push_back("-O");        // optimize for performance!
            pd.args.push_back("-o");
            if(target.empty()){
                pd.args.push_back("-");
            } else {
                pd.args.push_back(target.string());
            }
            
            if(source.empty()){
                pd.args.push_back("-");
            } else {
                pd.args.push_back(source.string());
            }
            
            ResultCC    ret;
            ret.payload  = pd.execute(input, &ret.errors, &ecode);
            ret.good    = ecode == 0;
            return ret;
        }
        
        ResultCC    validate_shader(const ByteArray&input, const std::filesystem::path& source)
        {
            static const path_vector_t          dirs    = shader_dirs();
            static const std::filesystem::path  glslv   = glsl_validator_executable();
            int                                 ecode   = -1;

            ProcessDescriptor   pd;
            pd.args.push_back(glslv.string());
            pd.args.push_back("--target-env");
            pd.args.push_back("vulkan1.2");
            for(auto& d : dirs){
                pd.args.push_back("-I" + d.string());
            }
            
            if(source.empty()){
                pd.args.push_back("-");
            } else {
                pd.args.push_back(source.string());
            }

            ResultCC    ret;
            ret.payload = pd.execute(input, &ret.errors, &ecode);
            ret.good    = ecode == 0;
            return ret;
        }
    }


    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    ShaderInfo::ShaderInfo(std::string_view zName, const AssetInfo& base, const std::source_location& sl) :
        AssetInfo(zName, base, sl)
    {
        set_option(SHADER);
    }

    ResultCC    Shader::compile(const std::filesystem::path& source, const std::filesystem::path& target)
    {
        return compile_shader(ByteArray(), source, target);
    }

    ResultCC    Shader::compile(const ByteArray& data)
    {
        return compile_shader(data, std::filesystem::path(), std::filesystem::path());
    }
    

    Ref<Shader>     Shader::get(const std::filesystem::path&fp)
    {
        return ShaderCache::singleton().get(fp);
    }
    
    Ref<Shader>     Shader::get(uint64_t i)
    {
        return ShaderCache::singleton().get(i);
    }

    ResultCC    Shader::validate(const std::filesystem::path& source)
    {
        return validate_shader(ByteArray(), source);
    }

    ResultCC    Shader::validate(const ByteArray& data)
    {
        return validate_shader(data, std::filesystem::path());
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


    ShaderCache&     ShaderCache::singleton()
    {
        static ShaderCache* s_singleton = new ShaderCache;
        return *s_singleton;
    }

    ////////////////////////////////////////////////////////////////////////////////

    ShaderCache::ShaderCache() : AssetCache(meta<ShaderCache>())
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
        ResultCC   compile(const std::filesystem::path& source, const std::filesystem::path& target) const 
        {
            return compile_shader(ByteArray(), source, target);
        }
    };
    
    
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    
    YQ_INVOKE(
        auto si = writer<Shader>();
        si.abstract();
        
        auto sca = writer<ShaderCache>();
        sca.abstract();
        sca.asset<Shader>();
        sca.compiler<ShaderCompiler>();
        sca.loader<ShaderLoader>();

        auto scc = writer<ShaderCompiler>();
        scc.abstract();

        auto sl = writer<ShaderLoader>();
        sl.abstract();
        
        auto glsl = writer<GLSLCompiler>();
        glsl.description("Shader compiler for GLSL language");
        glsl.extension("vert");
        glsl.extension("tesc");
        glsl.extension("tese");
        glsl.extension("frag");
        glsl.extension("geom");
        glsl.extension("comp");
        glsl.extension("mesh");
        glsl.extension("task");
        glsl.extension("rgen");
        glsl.extension("rint");
        glsl.extension("rahit");
        glsl.extension("rchit");
        glsl.extension("rmiss");
        glsl.extension("rcall");
    )
}

YQ_OBJECT_IMPLEMENT(yq::GLSLCompiler)



