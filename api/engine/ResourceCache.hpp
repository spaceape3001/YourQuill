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
#include <tbb/spin_rw_mutex.h>

namespace yq {
    namespace engine {
        class Resource;
        class ResourceCompiler;
        class ResourceCompilerInfo;
        class ResourceInfo;
        class ResourceLoader;
        class ResourceLoaderInfo;
        
        class ResourceCache;
        
        
        class ResourceCacheInfo : public ObjectInfo {
        public:
            template <typename C> class Writer;

            ResourceCacheInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl=std::source_location::current());

        protected:
        
            const ResourceInfo*            base_asset() const { return m_asset; }
            const ResourceCompilerInfo*    base_compiler() const { return m_compiler; }
            const ResourceLoaderInfo*      base_loader() const { return m_loader; }
        
        private:
            const ResourceInfo*            m_asset     = nullptr;
            const ResourceCompilerInfo*    m_compiler  = nullptr;
            const ResourceLoaderInfo*      m_loader    = nullptr;
        };
        
        /*! \brief An asset cache retains the data
        
            Most all engine components will query the cache for the data, if not present, loads it.  (Compiling first, if necessary)
        */
        class ResourceCache : public Object {
            YQ_OBJECT_INFO(ResourceCacheInfo)
            YQ_OBJECT_DECLARE(ResourceCache, Object)
        public:
        
        
        
            //  TODO.....

        protected:
            Ref<Resource>          get(const std::filesystem::path&);
            Ref<Resource>          get(uint64_t);


            ResourceCache(const ResourceCacheInfo&);      // can't rely on the virtual working.....

            virtual Ref<const Resource>  load_binary(const std::filesystem::path&) const = 0;
            
        private:
            std::vector<const ResourceInfo*>       m_assetInfos;
            std::vector<const ResourceLoader*>     m_assetLoaders;
            std::vector<const ResourceCompiler*>   m_assetCompilers;

            mutable tbb::spin_rw_mutex          m_mutex;
            std::map<std::filesystem::path, Ref<const Resource>>   m_cache;
        };

    }
}
