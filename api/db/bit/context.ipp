////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "context.hpp"

namespace yq {
    bool        read_xn(Context&ret, const XmlNode*xn)
    {
        ret.title   = read_attribute(xn, szTitle, x_string);
        ret.icon    = read_attribute(xn, szIcon, x_string);
        ret.order   = read_attribute(xn, szOrder, x_integer).value;
        ret.format  = read_attribute(xn, szFormat, x_enum<Format>).value;
        ret.data    = to_string(xn);
        return true;
    }

    Context     x_context(const XmlNode* xn)
    {
        Context ret;
        read_xn(ret, xn);
        return ret;
    }

    void                write_xn(XmlNode* xn, const Context&v)
    {
        if(v.format != Format())
            write_attribute(xn, szFormat, v.format);
        if(!v.title.empty())
            write_attribute(xn, szTitle, v.title);
        if(!v.icon.empty())
            write_attribute(xn, szIcon, v.icon);
        if(v.order)
            write_attribute(xn, szOrder, v.order);
        if(!v.data.empty()){
            xn->append_node(
                xn->document()->allocate_node(
                    rapidxml::node_cdata, nullptr, v.data.c_str(), 0ULL, v.data.size()
                )
            );
        }
    }
}
