////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/style/StyleProvider.hpp>
#include <mithril/style/StyleCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_styles()
    {
        return [](){
            return ids(cdb::all_styles(Sorted::YES));
        };
    }
}

