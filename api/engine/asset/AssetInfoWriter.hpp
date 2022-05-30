////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/meta/ObjectInfoWriter.hpp>
#include <engine/asset/Asset.hpp>
#include <engine/asset/AssetCache.hpp>
#include <engine/asset/AssetCompiler.hpp>
#include <engine/asset/AssetLoader.hpp>

namespace yq {
    template <typename C>
    class AssetInfo::Writer : public ObjectInfo::Writer<C> {
    public:
        Writer(AssetInfo* assetInfo) : ObjectInfo::Writer<C>(assetInfo)
        {
        }
        
        Writer(AssetInfo& assetInfo) : Writer(&assetInfo)
        {
        }
    };

    template <typename C>
    class AssetCacheInfo::Writer : public ObjectInfo::Writer<C> {
    public:
        Writer(AssetCacheInfo* assetCacheInfo) : ObjectInfo::Writer<C>(assetCacheInfo)
        {
        }
        
        Writer(AssetCacheInfo& assetCacheInfo) : Writer(&assetCacheInfo)
        {
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
