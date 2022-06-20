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
    class Asset;
    class AssetInfo;
    class AssetLoader;
    class AssetCache;
    

    class AssetInfo : public ObjectInfo {
    public:
        template <typename C> class Writer;

        AssetInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl=std::source_location::current());
        
        //! Loads the asset from native binary format (whatever that is)
        //virtual Ref<Asset>  load_binary(const std::filesystem::path&) { return nullptr; }
        
    protected:
    };
    
    /*! \brief An asset of the graphics engine
    
        An asset is something that can be loaded by the engine, and used in some predefined fashion.  
        (ie, texture, shader, sounds, etc)  
    */
    class Asset : public Object, public RefCount {
        YQ_OBJECT_INFO(AssetInfo)
        YQ_OBJECT_DECLARE(Asset, Object)
    public:
        
        virtual size_t      data_size() const = 0;
        
        //! Saves data to native binary format (whatever that is)
        virtual bool        save_binary(const std::filesystem::path&) const = 0;
        
        /*! \brief Asset ID
        
            This is the asset ID, as it resides in the asset cache.  
            If the ID is zero, then it's not from the asset cache.    
            A non-zero ID is guaranteed to be unique in the particular asset cache.
        */
        uint64_t            id() const { return m_id; }
        
        /*! Searches the given vector for the specified file
        
            \note An ABSOLUTE file will auto-return itself.
            
            This first checks the relative against the current working directory, returns if that file exists.
            Otherwise, it'll march through the paths, doing the same check.
        
            \param[in]  paths   Directories to search
            \param[in]  file    Given filepath
            \return filepath if absolute or detected, empty otherwise
        */
        static std::filesystem::path    search(const path_vector_t& paths, const std::filesystem::path& file);
        
    private:
        friend class AssetCache;
        uint64_t        m_id    = 0;
    };
}
