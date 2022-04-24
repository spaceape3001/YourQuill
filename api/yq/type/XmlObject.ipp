////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "XmlObject.hpp"
#include <yq/app/DelayInit.hpp>
#include <yq/meta/ObjectInfoWriter.hpp>

namespace yq {

    bool    XmlObject::load(const XmlNode*)
    {
        return true;
    }
    
    bool    XmlObject::save(XmlNode*) const
    {
        return true;
    }
    
}

YQ_OBJECT_IMPLEMENT(yq::XmlObject)

YQ_INVOKE(
    yq::writer<yq::XmlObject>().abstract();
)

