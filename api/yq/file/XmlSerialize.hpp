////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/file/XmlFwd.hpp>

namespace yq {

    // Base class for xml serialization
    struct XmlSerialize {
        virtual bool    load(const XmlNode*) { return true; }
        virtual bool    save(XmlNode*) const { return true; }
    };
}

