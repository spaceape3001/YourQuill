////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Object.hpp>
#include <basic/meta/ObjectInfo.hpp>
#include <basic/Ref.hpp>
#include <basic/UniqueID.hpp>

namespace yq {

    class MetaObjectInfo : public ObjectInfo {
    public:
        template <typename C> class Writer;
        
        MetaObjectInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl=std::source_location::current());
    };

    class MetaObject : public Object, public RefCount, public UniqueID {
        YQ_OBJECT_INFO(MetaObjectInfo)
        YQ_OBJECT_DECLARE(MetaObject, Object)
    public:
        uint64_t id() const { return UniqueID::id(); }
        
    protected:
        MetaObject();
        virtual ~MetaObject();
    };
}

