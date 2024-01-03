////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Field.hpp"
#include <0/basic/TextUtils.hpp>
#include <mithril/field/FieldCDB.hpp>

namespace yq::mithril {
    bool Field::less_key(Field a, Field b)
    {
        return is_less_igCase(cdb::key(a), cdb::key(b));
    }

    bool Field::less_label(Field a, Field b)
    {
        return is_less_igCase(cdb::label(a), cdb::label(b));
    }

    bool Field::less_name(Field a, Field b)
    {
        return is_less_igCase(cdb::name(a), cdb::name(b));
    }
}

