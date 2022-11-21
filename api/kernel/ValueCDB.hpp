////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/Field.hpp>
#include <kernel/Value.hpp>
//#include <kernel/CacheFwd.hpp>


namespace yq {

    struct Value::Info {
        Field       field;
        std::string brief, data;
    };

    namespace cdb {
        std::string             brief(Value);
        std::string             data(Value);
    
        Value              db_value(Field, std::string_view, bool* wasCreated=nullptr);
        
        bool                    exists(Value);
        bool                    exists_value(uint64_t);
        
        Field              field(Value);
        Value::Info        info(Value);
        
        Value              value(uint64_t);
        ValueVector        values(Field, Sorted=Sorted{});
        ValueSet           values_set(Field);
        
    }
}

