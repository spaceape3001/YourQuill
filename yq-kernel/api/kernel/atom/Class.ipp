////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Class.hpp"
#include "ClassCDB.hpp"
#include <basic/TextUtils.hpp>

namespace yq {
    bool Class::less_key(Class a, Class b)
    {
        return is_less_igCase(cdb::key(a), cdb::key(b));
    }

    bool Class::less_label(Class a, Class b)
    {
        return is_less_igCase(cdb::label(a), cdb::label(b));
    }

    bool Class::less_name(Class a, Class b)
    {
        return is_less_igCase(cdb::name(a), cdb::name(b));
    }
}
