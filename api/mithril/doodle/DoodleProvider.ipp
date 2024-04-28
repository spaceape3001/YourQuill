////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/doodle/DoodleProvider.hpp>
#include <mithril/doodle/DoodleCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_doodles()
    {
        return [](){
            return ids(cdb::all_doodles(Sorted::YES));
        };
    }
}

