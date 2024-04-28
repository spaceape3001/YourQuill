////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/root/Root.hpp>
#include <mithril/root/RootCDB.hpp>
#include <mithril/root/RootProvider.hpp>

namespace yq::mithril::provider {
    IdProvider all_roots()
    {
        return [](){
            return ids(cdb::all_roots());
        };
    }
}

