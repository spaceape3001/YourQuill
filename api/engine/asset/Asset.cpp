////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "AssetInfoWriter.hpp"
#include <basic/DelayInit.hpp>
#include <basic/DirUtils.hpp>
#include <config/DirConfig.hpp>

YQ_OBJECT_IMPLEMENT(yq::engine::Asset)
YQ_OBJECT_IMPLEMENT(yq::engine::AssetCache)
YQ_OBJECT_IMPLEMENT(yq::engine::AssetCompiler)
YQ_OBJECT_IMPLEMENT(yq::engine::AssetLoader)

namespace yq {
    namespace engine {
        
        namespace {
            path_vector_t               data_search_path()
            {
                //  we'll make this fancier (later)
                path_vector_t   ret;
                ret.push_back(build::data_directory());
                return ret;
            }
            
            std::filesystem::path       binary_cache_root()
            {
                return "/tmp/yquill/.binary";
            }
        
            //! Resolves the data file
            std::filesystem::path       resolve_data_file(const std::filesystem::path&p)
            {
                static const path_vector_t  s_paths = data_search_path();
                
                if(p.is_absolute())
                    return p;
                    
                for(const auto& d : s_paths){
                    std::filesystem::path   p2  = d / p;
                    if(std::filesystem::exists(p2))
                        return p2;
                }
                
                return std::filesystem::absolute(p);    //  last test
                
            }
            
            //!  Assuming input is absolute, creates a binary cache equivalent name
            std::filesystem::path       binary_cache_equivalent(const std::filesystem::path&p)
            {
                static const std::filesystem::path  b_cache = std::filesystem::path();
                return b_cache.string() + p.string();
            }
        }
        
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        AssetInfo::AssetInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl) :
            ObjectInfo(zName, base, sl)
        {
            set_option(ASSET);
        }

        std::filesystem::path    Asset::search(const path_vector_t& paths, const std::filesystem::path& fp)
        {
            if(fp.is_absolute())
                return fp;
            std::filesystem::path   p2  = std::filesystem::absolute(fp);
            if(std::filesystem::exists(p2))
                return p2;
            for(auto& d : paths){
                p2  = d / fp;
                if(std::filesystem::exists(p2))
                    return p2;
            }
            return std::filesystem::path();
        }

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