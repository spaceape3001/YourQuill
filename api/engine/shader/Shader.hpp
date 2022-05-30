////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/asset/Asset.hpp>
#include <engine/shader/ShaderType.hpp>
#include <basic/ByteArray.hpp>

namespace yq {
    class Shader;
    class ShaderCompiler;
    class ShaderLoader;
    class ShaderCache;
    struct ResultCC;

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
    

        /*! \brief Compiles a shader
        
            This compiles a shader, from the specified file, into the target.
            
            \param[in]  source  File path to the source
            \param[in]  target  File path to the target location (leave blank to return as payload)
        */
        static ResultCC compile(const std::filesystem::path& source, const std::filesystem::path& target=std::filesystem::path());

        /*! \brief Compiles a shader
        
            This compiles a shader from into the result's payload.
            
            \param[in] glslData     This is the GLSL shader data.
        */
        static ResultCC compile(const ByteArray& glslData);


        //!  All shader directories...
        static const path_vector_t&   directories();

        //!  Gets or loads specified shader
        static Ref<Shader>      get(const std::filesystem::path&);

        //!  Gets specified shader
        static Ref<Shader>      get(uint64_t);
        
        /*! \brief Compiles & Loads shader
        
            This compiles and loads the shaders (bypassing the cache).
        */
        static Ref<Shader>      load(const std::filesystem::path&);

        /*! \brief Compiles & Loads shader
        
            This compiles and loads the shaders (bypassing the cache).
        */
        static Ref<Shader>      load(const ByteArray& glsl, ShaderType);
        
        
        static std::filesystem::path    search(const std::filesystem::path&);

        /*! \brief Validates a shader
        
            This validates a shader (from file).
            \param[in]  source File path to the source
        */
        static ResultCC         validate(const std::filesystem::path& source);

        /*! \brief Validates a shader
        
            This validates a shader (from memory).

            \param[in] glslData     This is the GLSL shader data.
        */
        static ResultCC         validate(const ByteArray& glslData, ShaderType);


        const ByteArray&    payload() const { return m_payload; }
        virtual size_t      data_size() const override;
        virtual bool        save_binary(const std::filesystem::path&) const override;
    
        ShaderType          shader_type() const { return m_type; }
    
        Shader(const ByteArray&, ShaderType);
        Shader(ByteArray&&, ShaderType);
        virtual ~Shader();
    
    private:
        friend class ShaderCache;

        Shader();
        
        //! Raw load (from absolute file)
        static Ref<Shader>      do_load(const std::filesystem::path&, ShaderType st, bool do_compile);

        ByteArray       m_payload;
        ShaderType      m_type;
    };
    

}
