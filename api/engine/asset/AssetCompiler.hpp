////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Object.hpp>
#include <basic/Ref.hpp>
#include <basic/Set.hpp>
#include <basic/SizeTimestamp.hpp>
#include <filesystem>

namespace yq {
    namespace engine {
        class Asset;
        class AssetInfo;
        class AssetLoader;
        class AssetCache;
        struct ResultCC;
        

        class AssetCompilerInfo : public ObjectInfo {
        public:
            template <typename C> class Writer;

            AssetCompilerInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl=std::source_location::current());

            const StringSet&    extensions() const { return m_extensions; }

        protected:
            StringSet   m_extensions;
        };
        

        /*! \brief Compiler of asset
        
            An asset compiler is one that can convert a given resource into the internal representation
            within a file (instead of memory.  When a loader & compiler both claim ownership to 
            file extensions, the compiler will be given preference for FILE to FILE conversions.
        */
        class AssetCompiler : public Object {
            YQ_OBJECT_INFO(AssetCompilerInfo)
            YQ_OBJECT_DECLARE(AssetCompiler, Object)
        public:
        
            /*! \brief "Compiles" the asset
            
                This compiles the asset into a file of the native format.  "first" is success/failure, 
                while "second" is any relevant error output from an external program compiler.
            */
            virtual ResultCC    compile(const std::filesystem::path& source, const std::filesystem::path& target) const = 0;
        };

    }
}
