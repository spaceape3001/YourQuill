////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/tag/TagProvider.hpp>
#include <mithril/tag/TagCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_tags()
    {
        return [](){
            return ids(cdb::all_tags(Sorted::YES));
        };
    }
}

