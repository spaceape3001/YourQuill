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
    namespace engine {
        class Asset;
        class AssetCompiler;
        class AssetCompilerInfo;
        class AssetInfo;
        class AssetLoader;
        class AssetLoaderInfo;
        
        class AssetCache;
        
        
        class AssetCacheInfo : public ObjectInfo {
        public:
            template <typename C> class Writer;

            AssetCacheInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl=std::source_location::current());

        protected:
        
            const AssetInfo*            base_asset() const { return m_asset; }
            const AssetCompilerInfo*    base_compiler() const { return m_compiler; }
            const AssetLoaderInfo*      base_loader() const { return m_loader; }
        
        private:
            const AssetInfo*            m_asset     = nullptr;
            const AssetCompilerInfo*    m_compiler  = nullptr;
            const AssetLoaderInfo*      m_loader    = nullptr;
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


            AssetCache(const AssetCacheInfo&);      // can't rely on the virtual working.....

            virtual Ref<const Asset>  load_binary(const std::filesystem::path&) const = 0;
            
        private:
            std::vector<const AssetInfo*>       m_assetInfos;
            std::vector<const AssetLoader*>     m_assetLoaders;
            std::vector<const AssetCompiler*>   m_assetCompilers;
            
        };

    }
}
