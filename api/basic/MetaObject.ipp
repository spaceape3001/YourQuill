////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MetaObject.hpp"
#include "DelayInit.hpp"
#include "meta/ObjectInfoWriter.hpp"

namespace yq {
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

