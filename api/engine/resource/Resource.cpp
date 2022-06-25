////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Resource.hpp"
#include "ResourceInfoWriter.hpp"
#include <basic/DelayInit.hpp>
#include <basic/DirUtils.hpp>
#include <config/DirConfig.hpp>

YQ_OBJECT_IMPLEMENT(yq::engine::Resource)

namespace yq {
    namespace engine {
        namespace {
            path_vector_t               make_resource_search_path()
            {
                //  we'll make this fancier (later)
                path_vector_t   ret;
                ret.push_back(build::data_directory());
                return ret;
            }

            std::filesystem::path       make_binary_cache_root()
            {
                return "/tmp/yquill/.binary";
            }

        }

        
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        ResourceInfo::ResourceInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl) :
            ObjectInfo(zName, base, sl)
        {
            set_option(ASSET);
        }
        
        
        //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        const path_vector_t&             Resource::search_path()
        {
            static const path_vector_t sp = make_resource_search_path();
            return sp;
        }
        
        const std::filesystem::path&     Resource::binary_root()
        {
            static const std::filesystem::path  fp = make_binary_cache_root();
            return fp;
        }
        
        std::filesystem::path            Resource::resolve(const std::filesystem::path&p)
        {
            static path_vector_t  s_paths = search_path();
            if(p.is_absolute())
                return p;
                
            for(const auto& d : s_paths){
                std::filesystem::path   p2  = d / p;
                if(std::filesystem::exists(p2))
                    return p2;
            }
            
            return std::filesystem::absolute(p);    //  last test
        }
        
        std::filesystem::path            Resource::binary_path(const std::filesystem::path& p)
        {
            static const std::filesystem::path  b_cache = make_binary_cache_root();
            return b_cache.string() + p.string();
        }
        

        std::filesystem::path    Resource::search(const path_vector_t& paths, const std::filesystem::path& fp)
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

        Resource::Resource()
        {
        }
        
        Resource::~Resource()
        {
        }
        
        YQ_INVOKE(
            auto res = writer<Resource>();
            res.property("id", &Resource::id);
        )
    }
}
