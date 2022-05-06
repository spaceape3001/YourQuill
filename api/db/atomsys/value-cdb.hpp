////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/core/cdb_common.hpp>
#include <yq/atom/Field.hpp>
#include <yq/atom/Value.hpp>
#include <yq/enum/Sorted.hpp>

namespace yq {

    struct Value::Info {
        Field       field;
        std::string brief, data;
    };

    namespace cdb {
        std::string     brief(Value);
        std::string     data(Value);
    
        Value           db_value(Field, std::string_view, bool* wasCreated=nullptr);
        
        bool            exists(Value);
        bool            exists_value(uint64_t);
        
        Field           field(Value);
        Value::Info     info(Value);
        
        Value           value(uint64_t);
        Vector<Value>   values(Field, Sorted=Sorted{});
        Set<Value>      values_set(Field);
        
    }
}

