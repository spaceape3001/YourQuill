////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Object.hpp"
#include <yq/io/XmlSerialize.hpp>

namespace yq {
    class XmlObject : public Object, public XmlSerialize {
        YQ_OBJECT_DECLARE(XmlObject, Object)
    public:
        virtual bool    load(const XmlNode*) override;
        virtual bool    save(XmlNode*) const override;
    };
}
