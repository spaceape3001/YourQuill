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
        
        //! ID is unique....
        uint64_t            id() const { return m_id; }
        
    private:
        friend class AssetCache;
        uint64_t        m_id    = 0;
    };
}
