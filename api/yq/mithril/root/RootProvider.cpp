////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/root/Root.hpp>
#include <yq/mithril/root/RootCDB.hpp>
#include <yq/mithril/root/RootProvider.hpp>

namespace yq::mithril::provider {
    IdProvider all_roots()
    {
        return [](){
            return ids(cdb::all_roots());
        };
    }
}

