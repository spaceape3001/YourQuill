////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/drafter/DrafterProvider.hpp>
#include <mithril/drafter/DrafterCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_drafters()
    {
        return [](){
            return ids(cdb::all_drafters(Sorted::YES));
        };
    }
}

