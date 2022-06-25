////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "AssetInfoWriter.hpp"
#include <basic/DelayInit.hpp>
#include <basic/DirUtils.hpp>
#include <config/DirConfig.hpp>

YQ_OBJECT_IMPLEMENT(yq::engine::AssetCache)
YQ_OBJECT_IMPLEMENT(yq::engine::AssetCompiler)
YQ_OBJECT_IMPLEMENT(yq::engine::AssetLoader)

namespace yq {
    namespace engine {

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        AssetCacheInfo::AssetCacheInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl) :
            ObjectInfo(zName, base, sl)
        {
            set_option(CACHE);
        }

        AssetCache::AssetCache(const AssetCacheInfo&)      // can't rely on the virtual working.....
        {
        }
        

        /*
            DEFAULT BINARY location will be stored at /tmp/yquill/.binary, with the provided name tacked onto it.
            
            So, /home/user/foobar/mytexture.xpm -> /tmp/yquill/.binary/home/user/foobar/mytexture.xpm
        */

        Ref<Asset>          AssetCache::get(const std::filesystem::path& fp)
        {
            return Ref<Asset>();    // todo
        }
        
        Ref<Asset>          AssetCache::get(uint64_t i)
        {
            return Ref<Asset>();    // todo
        }

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        AssetCompilerInfo::AssetCompilerInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl) :
            ObjectInfo(zName, base, sl)
        {
            set_option(COMPILER);
        }

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        AssetLoaderInfo::AssetLoaderInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl) :
            ObjectInfo(zName, base, sl)
        {
            set_option(LOADER);
        }

        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    }
}
