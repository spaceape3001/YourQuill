////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/meta/Meta.hpp>

/*
    Now that we've made Meta an ID type, we need the CDB stuff too
*/

namespace yq::mithril::cdb {
    std::vector<Meta>   all_metas(Sorted sorted=Sorted{});
    std::string         key(Meta);
    std::string         name(Meta);
}
