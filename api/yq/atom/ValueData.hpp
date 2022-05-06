////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/atom/Value.hpp>
#include <yq/collection/Vector.hpp>

namespace yq {
    struct Value::Data {
        std::string     brief, notes, data;
        
        Data&   merge(const Data&, bool fOverride);
    };
    
    Value::Data x_value(const XmlNode*);
    bool    read_xn(Value::Data&, const XmlNode*);
    void    write_xn(XmlNode*, const Value::Data&);
    
    struct Value::ListData {
        Vector<Data>   values;

        ListData&   merge(const ListData&, bool fOverride);
    };
    
}
