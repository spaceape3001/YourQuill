////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/enum/Format.hpp>
#include <basic/preamble.hpp>
#include <vector>

namespace yq {
    struct Root;
    class Stream;

    struct Context {
        std::string     icon, title, data;
        const Root*     root = nullptr;
        Format          format;
        int             order=0;
        
        bool        operator==(const Context&) const = default;
    };

    bool       read_kv(std::vector<Context>&, std::string_view);
    void       write_kv(Stream&, const std::vector<Context>&);

    Context         x_context(const XmlNode*);
    bool            read_xn(Context&, const XmlNode*);
    void            write_xn(XmlNode* xb, const Context&);
}
