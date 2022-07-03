////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/MetaObject.hpp>
#include <basic/meta/ObjectInfoWriter.hpp>

namespace yq {
    template <typename C>
    class MetaObjectInfo::Writer : public ObjectInfo::Writer<C> {
    public:
        Writer(MetaObjectInfo* metaObjInfo) : ObjectInfo::Writer<C>(metaObjInfo)
        {
        }
        
        Writer(MetaObjectInfo& metaObjInfo) : Writer(&metaObjInfo)
        {
        }
    };
}
