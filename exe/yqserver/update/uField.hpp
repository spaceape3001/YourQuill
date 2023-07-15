////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <update/preamble.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/field/FieldData.hpp>

namespace yq::mithril::update {
    struct UField : public U<Field> {
        static UField&      get(Field f);
        
        const Document      doc;
        //Field::SharedData   def;

        UField(Field);
        void                flash(FF);
    };
}
