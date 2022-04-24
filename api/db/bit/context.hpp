////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/enum/format.hpp>
#include <yq/file/XmlFwd.hpp>

namespace yq {

    struct Context {
        std::string     icon, title, data;
        Format          format;
        int             order=0;
        
        auto        operator<=>(const Context&) const = default;
    };

    Context         x_context(const XmlNode*);
    bool            read_xn(Context&, const XmlNode*);
    void            write_xn(XmlNode* xb, const Context&);
}
