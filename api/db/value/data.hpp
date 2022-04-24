////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "struct.hpp"

namespace yq {
    struct Value::Data {
        std::string     brief, notes, data;
        
        Data&   merge(const Data&, bool fOverride);
    };
    
    Value::Data x_value(const XmlNode*);
    bool    read(Value::Data&, const XmlNode*);
    void    write(XmlNode*, const Value::Data&);
}
