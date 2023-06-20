////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/AtomProvider.hpp>
#include <mithril/atom/AtomCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_atoms()
    {
        return [](){
            return ids(cdb::all_atoms(Sorted::YES));
        };
    }
}

