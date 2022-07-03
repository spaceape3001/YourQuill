////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MetaObject.hpp"
#include "DelayInit.hpp"
#include "MetaObjectInfoWriter.hpp"

namespace yq {
    MetaObjectInfo::MetaObjectInfo(std::string_view zName, const ObjectInfo& base, const std::source_location& sl) : 
        ObjectInfo(zName, base, sl)
    {
    }

    MetaObject::MetaObject()
    {
    }
    
    MetaObject::~MetaObject()
    {
    }

    YQ_INVOKE(
        auto mo = writer<MetaObject>();
        mo.property("id", &MetaObject::id);
    )
}

YQ_OBJECT_IMPLEMENT(yq::MetaObject)

