////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/MetaObject.hpp>
#include <basic/Ref.hpp>
#include <basic/Set.hpp>
#include <basic/SizeTimestamp.hpp>
#include <basic/UniqueID.hpp>
#include <filesystem>

namespace yq {
    namespace engine {
        class Asset;
        class AssetInfo;
        class AssetLoader;
        class AssetCache;
        

        class AssetInfo : public MetaObjectInfo {
        public:
            template <typename C> class Writer;

            AssetInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl=std::source_location::current());
            
            //! Loads the asset from native binary format (whatever that is)
            //virtual Ref<Asset>  load_binary(const std::filesystem::path&) { return nullptr; }
            
        protected:
        };
        
        /*! \brief An asset of the graphics engine
        
            An asset (here) is something that can be loaded by the engine, and used in some predefined fashion.  
            (ie, texture, shader, sounds, etc)  Which is why the asset-library also exists, predefined cameras & shapes
        */
        class Asset : public MetaObject {
            YQ_OBJECT_INFO(AssetInfo)
            YQ_OBJECT_DECLARE(Asset, MetaObject)
        public:
            virtual size_t      data_size() const = 0;
            
            //! Saves data to native binary format (whatever that is)
            virtual bool        save_binary(const std::filesystem::path&) const = 0;
            
            static const path_vector_t&             search_path();
            static const std::filesystem::path&     binary_root();
            static std::filesystem::path            resolve(const std::filesystem::path&);
            static std::filesystem::path            binary_path(const std::filesystem::path&);
            
            /*! Searches the given vector for the specified file
            
                \note An ABSOLUTE file will auto-return itself.
                
                This first checks the relative against the current working directory, returns if that file exists.
                Otherwise, it'll march through the paths, doing the same check.
            
                \param[in]  paths   Directories to search
                \param[in]  file    Given filepath
                \return filepath if absolute or detected, empty otherwise
            */
            static std::filesystem::path            search(const path_vector_t& paths, const std::filesystem::path& file);
        
        protected:
            Asset();
            virtual ~Asset();
        };
    }
}
