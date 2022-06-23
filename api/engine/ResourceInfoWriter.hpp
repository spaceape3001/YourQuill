////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/meta/ObjectInfoWriter.hpp>
#include <engine/Resource.hpp>
#include <engine/ResourceCache.hpp>
#include <engine/ResourceCompiler.hpp>
#include <engine/ResourceLoader.hpp>

namespace yq {
    namespace engine {
        template <typename C>
        class ResourceInfo::Writer : public ObjectInfo::Writer<C> {
        public:
            Writer(ResourceInfo* assetInfo) : ObjectInfo::Writer<C>(assetInfo)
            {
            }
            
            Writer(ResourceInfo& assetInfo) : Writer(&assetInfo)
            {
            }
        };

        template <typename C>
        class ResourceCacheInfo::Writer : public ObjectInfo::Writer<C> {
        public:
            Writer(ResourceCacheInfo* assetCacheInfo) : ObjectInfo::Writer<C>(assetCacheInfo)
            {
            }
            
            Writer(ResourceCacheInfo& assetCacheInfo) : Writer(&assetCacheInfo)
            {
            }
            
            Writer&     asset(const ResourceInfo& ai)
            {
                ResourceCacheInfo* aci = static_cast<ResourceCacheInfo*>(Meta::Writer::m_meta);
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
            
            Writer&     compiler(const ResourceCompilerInfo&ai)
            {
                ResourceCacheInfo* aci = static_cast<ResourceCacheInfo*>(Meta::Writer::m_meta);
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
            
            Writer& loader(const ResourceLoaderInfo& ali)
            {
                ResourceCacheInfo* aci = static_cast<ResourceCacheInfo*>(Meta::Writer::m_meta);
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
        class ResourceCompilerInfo::Writer : public ObjectInfo::Writer<C> {
        public:
            Writer& extension(std::string_view v)
            {
                ResourceCompilerInfo*     ali = static_cast<ResourceCompilerInfo*>(Meta::Writer::m_meta);
                if(ali)
                    ali -> m_extensions.insert(std::string(v));
                return *this;
            }

            Writer(ResourceCompilerInfo* assetCompilerInfo) : ObjectInfo::Writer<C>(assetCompilerInfo)
            {
            }
            
            Writer(ResourceCompilerInfo& assetCompilerInfo) : Writer(&assetCompilerInfo)
            {
            }
        };

        template <typename C>
        class ResourceLoaderInfo::Writer : public ObjectInfo::Writer<C> {
        public:
            
            Writer& extension(std::string_view v)
            {
                ResourceLoaderInfo*     ali = static_cast<ResourceLoaderInfo*>(Meta::Writer::m_meta);
                if(ali)
                    ali -> m_extensions.insert(std::string(v));
                return *this;
            }
        
        
            Writer(ResourceLoaderInfo* assetLoaderInfo) : ObjectInfo::Writer<C>(assetLoaderInfo)
            {
            }
            
            Writer(ResourceLoaderInfo& assetLoaderInfo) : Writer(&assetLoaderInfo)
            {
            }
        };
    }
}
