////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/field/FieldProvider.hpp>
#include <mithril/field/FieldCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_fields()
    {
        return [](){
            return ids(cdb::all_fields(Sorted::YES));
        };
    }
}

