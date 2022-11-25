////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/agw/Value.hpp>
#include <basic/Vector.hpp>

namespace yq {
    namespace agw {
        struct Value::Data {
            std::string     brief, notes, data;
            
            Data&   merge(const Data&, bool fOverride);
        };
        
        struct Value::ListData {
            Vector<Data>   values;

            ListData&   merge(const ListData&, bool fOverride);
        };

    Value::Data    x_value(const XmlNode*);
    bool           read_xn(Value::Data&, const XmlNode*);
    void           write_xn(XmlNode*, const Value::Data&);
    
    }
}
