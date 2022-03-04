////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <db/enum/Format.hpp>
#include <util/file/XmlFwd.hpp>

namespace yq {

    struct Context {
        Format      format;
        std::string data;
        
        auto        operator<=>(const Context&) const = default;
    };

    Context         x_context(const XmlNode*);
    void            write_xn(XmlNode* xb, const Context&);
}
