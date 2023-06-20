////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/root/RootProvider.hpp>
#include <mithril/root/RootCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_roots()
    {
        return [](){
            return ids(cdb::all_roots());
        };
    }
}

