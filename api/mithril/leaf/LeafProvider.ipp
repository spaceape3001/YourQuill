////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/leaf/LeafProvider.hpp>
#include <mithril/leaf/LeafCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_leafs()
    {
        return [](){
            return ids(cdb::all_leafs(Sorted::YES));
        };
    }
}

