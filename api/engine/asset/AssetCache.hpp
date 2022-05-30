////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Object.hpp>
#include <basic/Ref.hpp>
#include <basic/Set.hpp>
#include <filesystem>

namespace yq {
    class Asset;
    class AssetInfo;
    class AssetLoader;
    class AssetCache;
    
    
    class AssetCacheInfo : public ObjectInfo {
    public:
        template <typename C> class Writer;

        AssetCacheInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl=std::source_location::current());

    protected:
    };
    
    /*! \brief An asset cache retains the data
    
        Most all engine components will query the cache for the data, if not present, loads it.  (Compiling first, if necessary)
    */
    class AssetCache : public Object {
        YQ_OBJECT_INFO(AssetCacheInfo)
        YQ_OBJECT_DECLARE(AssetCache, Object)
    public:
    
    
        //  TODO.....

    protected:
        Ref<Asset>          get(const std::filesystem::path&);
        Ref<Asset>          get(uint64_t);


        virtual Ref<Asset>  load_binary(const std::filesystem::path&) const = 0;
    };

}
