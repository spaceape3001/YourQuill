////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <update/preamble.hpp>
#include <mithril/field/FieldData.hpp>

namespace yq::mithril::update {
    struct UField {
        static UField&      get(Field f);
        
        const Field         field;
        const uint64_t      id;
        const std::string   key;
        Field::SharedData   def;
        
        UField(Field);
    };
}
