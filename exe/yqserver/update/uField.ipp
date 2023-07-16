////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uField.hpp"
#include <mithril/field/FieldCDB.hpp>

namespace yq::mithril::update {
    UField&  UField::get(Field x)
    {
        return lookup<UField>(x);
    }

    UField::UField(Field x) : U<Field>(x, cdb::key(x)), doc(cdb::document(x))
    {
    }
}

