////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/agw/Field.hpp>
#include <kernel/agw/Value.hpp>
//#include <kernel/db/CacheFwd.hpp>
#include <kernel/enum/Sorted.hpp>

namespace yq {

    namespace agw {

        struct Value::Info {
            Field       field;
            std::string brief, data;
        };
    }

    namespace cdb {
        std::string             brief(agw::Value);
        std::string             data(agw::Value);
    
        agw::Value              db_value(agw::Field, std::string_view, bool* wasCreated=nullptr);
        
        bool                    exists(agw::Value);
        bool                    exists_value(uint64_t);
        
        agw::Field              field(agw::Value);
        agw::Value::Info        info(agw::Value);
        
        agw::Value              value(uint64_t);
        agw::ValueVector        values(agw::Field, Sorted=Sorted{});
        agw::ValueSet           values_set(agw::Field);
        
    }
}

