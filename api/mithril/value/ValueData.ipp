////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ValueData.hpp"

#include <yq/io/XmlUtils.hpp>
#include <mithril/io/Strings.hpp>

namespace yq::mithril {

    Value::Data&   Value::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(notes, b.notes, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(data, b.data, fOverride);
        return *this;
    }

    
    Value::Data x_value(const XmlNode&xn)
    {
        Value::Data ret;
        read_xn(ret, xn);
        return ret;
    }
    
    bool    read_xn(Value::Data&ret, const XmlNode&xn)
    {
        ret.notes   = read_child(xn, szNotes, x_string);
        ret.brief   = read_child(xn, szBrief, x_string);
        ret.data    = read_child(xn, szData, x_string);
        return true;
    }
    
    void    write_xn(XmlNode&xn, const Value::Data&v)
    {
        write_child(xn, szData, v.data);
        if(!v.brief.empty())
            write_child(xn, szBrief, v.brief);
        if(!v.notes.empty())
            write_child(xn, szNotes, v.notes);
    }

    Value::ListData&   Value::ListData::merge(const ListData&b, bool fOverride)
    {
        values += b.values;
        return *this;
    }
}
