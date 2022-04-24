////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "data.hpp"

namespace yq {

    Value::Data&   Value::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(notes, b.notes, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(data, b.data, fOverride);
        return *this;
    }

    
    Value::Data x_value(const XmlNode*xn)
    {
        Value::Data ret;
        read(ret, xn);
        return ret;
    }
    
    bool    read(Value::Data&ret, const XmlNode*xn)
    {
        ret.notes   = read_child(xn, szNotes, x_sstring);
        ret.brief   = read_child(xn, szBrief, x_sstring);
        ret.data    = read_child(xn, szData, x_sstring);
        return true;
    }
    
    void    write(XmlNode*xn, const Value::Data&v)
    {
        write_child(xn, szData, v.data);
        if(!v.brief.empty())
            write_child(xn, szBrief, v.brief);
        if(!v.notes.empty())
            write_child(xn, szNotes, v.notes);
    }
}

