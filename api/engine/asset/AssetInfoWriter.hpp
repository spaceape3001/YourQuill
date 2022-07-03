////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/asset/Asset.hpp>
#include <engine/asset/AssetCache.hpp>
#include <engine/asset/AssetCompiler.hpp>
#include <engine/asset/AssetLoader.hpp>

namespace yq {
    namespace engine {

        template <typename C>
        class AssetCacheInfo::Writer : public ObjectInfo::Writer<C> {
        public:
            Writer(AssetCacheInfo* assetCacheInfo) : ObjectInfo::Writer<C>(assetCacheInfo)
            {
            }
            
            Writer(AssetCacheInfo& assetCacheInfo) : Writer(&assetCacheInfo)
            {
            }
            
            Writer&     asset(const AssetInfo& ai)
            {
                AssetCacheInfo* aci = static_cast<AssetCacheInfo*>(Meta::Writer::m_meta);
                if(aci)
                    aci -> m_asset  = &ai;
                return *this;
            }
            
            template <typename A>
            Writer&     asset()
            {
                asset(meta<A>());
                return *this;
            }
            
            Writer&     compiler(const AssetCompilerInfo&ai)
            {
                AssetCacheInfo* aci = static_cast<AssetCacheInfo*>(Meta::Writer::m_meta);
                if(aci)
                    aci -> m_compiler = &ai;
                return *this;
            }
            
            
            template <typename A>
            Writer&     compiler()
            {
                compiler(meta<A>());
                return *this;
            }
            
            Writer& loader(const AssetLoaderInfo& ali)
            {
                AssetCacheInfo* aci = static_cast<AssetCacheInfo*>(Meta::Writer::m_meta);
                if(aci)
                    aci -> m_loader = &ali;
                return *this;
            }
            
            template <typename A>
            Writer&     loader()
            {
                loader(meta<A>());
                return *this;
            }
        };

        template <typename C>
        class AssetCompilerInfo::Writer : public ObjectInfo::Writer<C> {
        public:
            Writer& extension(std::string_view v)
            {
                AssetCompilerInfo*     ali = static_cast<AssetCompilerInfo*>(Meta::Writer::m_meta);
                if(ali)
                    ali -> m_extensions.insert(std::string(v));
                return *this;
            }

            Writer(AssetCompilerInfo* assetCompilerInfo) : ObjectInfo::Writer<C>(assetCompilerInfo)
            {
            }
            
            Writer(AssetCompilerInfo& assetCompilerInfo) : Writer(&assetCompilerInfo)
            {
            }
        };

        template <typename C>
        class AssetLoaderInfo::Writer : public ObjectInfo::Writer<C> {
        public:
            
            Writer& extension(std::string_view v)
            {
                AssetLoaderInfo*     ali = static_cast<AssetLoaderInfo*>(Meta::Writer::m_meta);
                if(ali)
                    ali -> m_extensions.insert(std::string(v));
                return *this;
            }
        
        
            Writer(AssetLoaderInfo* assetLoaderInfo) : ObjectInfo::Writer<C>(assetLoaderInfo)
            {
            }
            
            Writer(AssetLoaderInfo& assetLoaderInfo) : Writer(&assetLoaderInfo)
            {
            }
        };
    }
}
