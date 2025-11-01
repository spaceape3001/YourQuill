////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <yq/mithril/field/Field.hpp>
#include <yq/mithril/value/Value.hpp>

namespace yq::mithril {
    struct Value::Info {
        Field       field;
        std::string brief, data;
    };

    namespace cdb {
        
        std::vector<Value>      all_values(Sorted sorted=Sorted{});
    
        std::string             brief(Value);
        std::string             data(Value);
    
        Value                   db_value(Field, std::string_view, bool* wasCreated=nullptr);
        
        bool                    exists(Value);
        bool                    exists_value(uint64_t);
        
        Field                   field(Value);
        Value::Info             info(Value);
        
        std::string             key(Value);
        std::string             name(Value);
        Image                   icon(Value);
        
        Value                   value(uint64_t);
        std::vector<Value>      values(Field, Sorted=Sorted{});
        std::set<Value>         values_set(Field);
        
    }
}

