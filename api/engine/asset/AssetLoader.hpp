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
    namespace engine {
        class Asset;
        class AssetInfo;
        class AssetLoader;
        class AssetCache;
        

        class AssetLoaderInfo : public ObjectInfo {
        public:
            template <typename C> class Writer;

            AssetLoaderInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl=std::source_location::current());

            const StringSet&    extensions() const { return m_extensions; }

        protected:
            StringSet   m_extensions;
            
        };

        /*! \brief A loader LOADS an asset
        
            This function LOADS an asset from file into INTERNAL memory
        */
        class AssetLoader : public Object {
            YQ_OBJECT_INFO(AssetLoaderInfo)
            YQ_OBJECT_DECLARE(AssetLoader, Object)
        public:
        
            virtual Ref<Asset>      load_asset(const std::filesystem::path&) const = 0;
        };
        

    }
}
