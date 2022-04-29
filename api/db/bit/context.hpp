////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/enum/format.hpp>
#include <yq/file/XmlFwd.hpp>

namespace yq {
    struct Root;

    struct Context {
        std::string     icon, title, data;
        const Root*     root = nullptr;
        Format          format;
        int             order=0;
        
        bool        operator==(const Context&) const = default;
    };

    Context         x_context(const XmlNode*);
    bool            read_xn(Context&, const XmlNode*);
    void            write_xn(XmlNode* xb, const Context&);
}
